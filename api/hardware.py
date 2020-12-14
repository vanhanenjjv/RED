from json.decoder import JSONDecodeError
from logging import error
from typing import Any, Callable, Dict, NamedTuple, Union
from threading import Thread
from serial import Serial
from time import sleep
from models import Card
import json
from types import SimpleNamespace
from serial.tools.list_ports import comports
from state import state


# should move somewhere else
NFC_CARD                  = 0x10 
NFC_AUTHORIZE_CARD        = 0x11
NFC_READ_CARD             = 0x00 
NFC_AUTHENTICATION_RESULT = 0x01

Receiver = Callable[[str], None]

class Arduino:
    def __init__(self, port: str, baud_rate: int) -> None:
        # Serial config
        self.port      = port
        self.baud_rate = baud_rate
        self.serial    = Serial()

        # Serial reader callbacks
        self.receivers: Dict[int, Receiver] = {}

        self.nfc = NFC(self)

        # Serial reader thread
        self.receiver        = Thread(target=self.__receiver_loop,  args=())
        self.receiver.daemon = True
  
    def start(self, start_receiver=True) -> None:
        self.serial.port     = self.port
        self.serial.baudrate = self.baud_rate

        self.serial.open()

        if start_receiver:
            self.receiver.start()
    
    def transmit(self, message: Any) -> None:
        if not self.serial.is_open:
            error("Serial is not open.")
            return

        if message is not str:
            message = json.dumps(message)

        data = bytes(message, "ASCII")

        self.serial.write(data)

    def __receiver_loop(self) -> None:
        while self.serial.is_open:
            line = self.serial.readline().decode().rstrip('\n')

            try:
                message = json.loads(line, object_hook=lambda d: SimpleNamespace(**d))

                if hasattr(message, "type"):
                    receiver = self.receivers[message.type]
                    receiver(message)
                else:
                    print(f"[Arduino] {line}")                

            except JSONDecodeError:
                print(f"[Arduino] {line}")                

class NFC:
    def __init__(self, arduino: Arduino) -> None:
        self.arduino = arduino
        self.current_card: Union[Card, None] = None
        self.arduino.receivers[NFC_CARD] = self.__receive_card
        self.arduino.receivers[NFC_AUTHORIZE_CARD] = self.__authenticate_card

    def __authenticate_card(self, message: SimpleNamespace) -> None:
        authorized = message.card.uid in state.cards

        self.arduino.transmit({
            "type": NFC_AUTHENTICATION_RESULT,
            "authorized": authorized
        })

    def __receive_card(self, message: SimpleNamespace) -> None:
        card = Card(message.card.uid)
        self.current_card = card
        
    # yo I heard you like some multithreading issues
    def read_card(self) -> Card:
        self.current_card = None

        self.arduino.transmit({
            "type": NFC_READ_CARD
        })

        while self.current_card is None: # ripperino blockerino
            sleep(0.1)

        return self.current_card


ports = comports()

if len(ports) == 0:
    raise Exception("Serial port not found.")

arduino = Arduino(port=ports[0].device, baud_rate=115200)
