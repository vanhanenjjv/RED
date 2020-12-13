/*
  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 6
 * LCD D5 pin to digital pin 5
 * LCD D6 pin to digital pin 4
 * LCD D7 pin to digital pin 3
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 *LED to D9
 *Button to D2
*/

// include the library code: 
#include <LiquidCrystal.h> 
#include <Arduino.h>
#include <Adafruit_PN532.h>

#define BAUD_RATE 114200

#define PN532_IRQ   (2)
#define PN532_RESET (3)

Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

bool nfc_enabled = true;



LiquidCrystal lcd(12, 11, 7, 6, 5, 4);


const uint8_t ledPin = PB1;  	//LED D9
const uint8_t interruptPin = 3;  	//Button D2

//volatiles
//how long the led should light, 1 = 500 ms
volatile int ledControl = 0;
volatile const uint8_t uid_length = 7;




//Timer1 reset and timer compA goal
const uint16_t tl_load = 0;
const uint16_t tl_comp = 31250;




void setup() {

  //init watchdog timer
  WDT_Prescaler_Change();
  //__watchdog_reset(); must be called periodically


  lcd.begin(16, 2);
  Serial.begin(BAUD_RATE);

  //set ledPin to output
  DDRB |= (1 << ledPin);

  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("Didn't find PN53x board");
    while (1); // halt
  }
  nfc.setPassiveActivationRetries(0x00);
  nfc.SAMConfig();


  //Button interrupt routine
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), buttonPressed, RISING);


  timer1_setup();
}


void loop() {
 __watchdog_reset();
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };



  if (nfc_enabled) {
    if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uid_length)) {

      //check if UID is on EEPROM
      if(checkUID(uid)){
        //UID found - do things
        ledControl = 10;
      } else {
        //UID not found - do other things
        ledControl = -10;
      }
    }
  }
    delay(100);
}








//Runs every 500 ms
ISR(TIMER1_COMPA_vect) {
    //reset timer
    TCNT1 = tl_load;

    if (ledControl > 0) {
        //Check if led is off
        if (!(PINB & (1 << 1))) {
            PORTB ^= (1 << ledPin);
        }
        ledControl--;
    }
    else if (ledControl < 0)
    {
        //check if led is on
        if (PINB & (1 << 1)) {
            PORTB ^= (1 << ledPin);
        }
        ledControl++;
    }
    else {
        //if ledControl = 0, keep blinking
        PORTB ^= (1 << ledPin);
    }
}




/*
Button to interrupt UID read
Used to write new accepted UIDs to memory

*EEPROM - UID is uid_length(4 or 7) bytes, EEPROM is 1024 bytes
*Use first byte of EEPROM for UID address control
*/
void buttonPressed() {

  uint8_t newUid[] = {0,0,0,0,0,0,0};
  
  //read UID
  if (nfc_enabled) {
    if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &newUid[0], &uid_length)) {
      //check if UID is already in EEPROM
      if(checkUID(newUid)){
      //UID already found in EEPROM
      } else {
        //NEW UID FOUND - write it to EEPROM
        for(int i; i < uid_length; i++){
        //Write the first byte of new UID to lastaddress+1
        EEPROM_write(((numberOfUID*uid_length+1), newUid[i]))
        }
        //increment saved UIDs
        int num = EEPROM_read(0);
        EEPROM_write(0, num+1);
      }
    }
  }

}





void WDT_Prescaler_Change(void)
{
__disable_interrupt();
__watchdog_reset();
/* Start timed equence */
WDTCSR |= (1<<WDCE) | (1<<WDE);
/* Set new prescaler(time-out) value = 1024K cycles (~8 s) */
WDTCSR = (1<<WDE) | (1<<WDP3) | (1<<WDP0);
__enable_interrupt();
}



void EEPROM_write(unsigned int uiAddress, unsigned char ucData)
{
/* Wait for completion of previous write */
while(EECR & (1<<EEPE));
/* Set up address and Data Registers */
EEAR = uiAddress;
EEDR = ucData;
/* Write logical one to EEMPE */
EECR |= (1<<EEMPE);
/* Start eeprom write by setting EEPE */
EECR |= (1<<EEPE);
}

unsigned char EEPROM_read(unsigned int uiAddress)
{
/* Wait for completion of previous write */
while(EECR & (1<<EEPE));
/* Set up address register */
EEAR = uiAddress;
/* Start eeprom read by writing EERE */
EECR |= (1<<EERE);
/* Return data from Data Register */
return EEDR;
}




//returns true if UID found on EEPROM
bool checkUID(uint8_t uid[]){
  //Used to compare bytes of new UID to those already saved
  int numCheck = 0;
   
   if(EEPROM_read(0) > 0){
      //go through every saved UID
      for(int k = 0; k < numberOfUid; k++){
        
        //compare every byte of saved UID at address j+k*uid_length to those of newUID
        for(int j = 0; j < uid_length; j++){
          if(newUid[j] == EEPROM_read((j+k*uid_length)){
            numCheck++;
          }
        }
        if(numCheck == uid_length){
        return true;
        }

        //reset numCheck if it didn't hit uid_length
        numCheck = 0;
      }
    }
return false;
}


void timer1_setup(){
  //reset timer1 control reg A
  TCCR1A = 0;

  //prescaler 256
  TCCR1B |= (1 << CS12);
  TCCR1B &= ~(1 << CS11);
  TCCR1B &= ~(1 << CS10);

  //reset timer 1 and set compare value
  TCNT1 = tl_load;
  OCR1A = tl_comp;

  //enable timer1 compare interrupt
  TIMSK1 = (1 << OCIE1A);

  //enable global interrupt
  sei();
}

