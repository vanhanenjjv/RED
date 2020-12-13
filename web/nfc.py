from arduino import arduino

def disable() -> None:
    arduino.transmit({
        "type": 0
    })

def enable() -> None:
    arduino.transmit({
        "type": 1
    })

