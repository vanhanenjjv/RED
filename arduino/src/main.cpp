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
#include <EEPROM.h>


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 6, 5, 4, 3);


const byte ledPin = PB1;  	//LED D9
const byte interruptPin = 2;  	//Button D2


//volatiles
//how long the led should light, 1 = 500 ms
volatile int ledControl = 0;




//Timer
const uint16_t tl_load = 0;
const uint16_t tl_comp = 31250;




void setup() {


  //init watchdog timer
  //__watchdog_reset(); must be called periodically
  WDT_Prescaler_Change();










    lcd.begin(16, 2);
    Serial.begin(9600);

    //Button interrupt routine
    pinMode(interruptPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(interruptPin), buttonPressed, RISING);


    //ledPin output
    DDRB |= (1 << ledPin);


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


void loop() {

    bool uid_success = false;


/*UID READ
*If UID read && accepted, set ledControl to 5(1=500ms)
*save/send used UID and timestamp to webservice

*If UID read && unaccepted, set ledControl negative(led goes dark for x*500ms)
*save/send used UID and timestamp to webservice
*/
    if (uid_success) {
        ledControl = 5;
    }
    delay(100);
}








//Runs every 500 ms
ISR(TIMER1_COMPA_vect) {
    //reset timer
    TCNT1 = tl_load;

    if (ledControl > 0) {
        //if uid accepted, burn led

        //Check if led is off
        if (!(PINB & (1 << 1))) {
            PORTB ^= (1 << ledPin);
        }
        ledControl--;
    }
    else if (ledControl < 0)
    {
        //if uid unaccepted, shut led

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

*EEPROM - UID is 7 bytes, EEPROM is 1024 bytes
*Use first bytes of UID for address control


Use keypad to lock the adding of new UIDs if there's time
*/
void buttonPressed() {
  //Used to compare bytes of new UID to those already saved
  int numCheck = 0;
  
  ledControl = 10;
  byte newUid[7] = {0,0,0,0,0,0,0};
  byte numberOfUID = 0; //How many UIDS saved, Saved in EEPROM address 0
  //Read UID
  if(readSuccess){



    //check if UID is already in EEPROM
    if(numberOfUID > 0){
      //go through every saved UID
      for(int k = 0; k < numberOfUid; k++){
        
        //compare every byte of saved UID at address j+k*7 to those of newUID
        for(int j = 0; j < 7; j++){
          if(newUid[j] == EEPROM.read(j+k*7){
            numCheck++;
          }
        }
        
        
        if(numCheck == 7){
        //UID same as the last one saved
        }

        //reset numCheck if it didn't hit 7
        numCheck = 0;
      }
    }



    if(UID == FOUND){
      //Don't need to save same UID twice, ignore it
    
    } else {
      //NEW UID FOUND - write it to EEPROM

      for(int i; i < 7; i++){
        //Write the first byte of new UID to lastaddress+1
        EEPROM.write(((numberOfUID*7+1), newUid[i])
      }
      //increment saved UIDs
      numberOfUID++;
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