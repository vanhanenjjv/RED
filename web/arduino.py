from logging import error, info
from typing import Any, Callable, Union
from threading import Thread
from serial import Serial
from serial.tools.list_ports import comports
import json

ReceiveCallback = Callable[[str], None]

class Arduino:
    def __init__(self, port: str, baud_rate: int) -> None:
        # Serial config
        self.port      = port
        self.baud_rate = baud_rate
        self.serial    = Serial()

        # Serial reader callback
        self.receive: Union[ReceiveCallback, None] = None

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

            if self.receive is not None:
                self.receive(line)


ports = comports()

if len(ports) == 0:
    raise Exception("Serial port not found.")

arduino = Arduino(
    port=ports[0].device,
    baud_rate=114200
)
