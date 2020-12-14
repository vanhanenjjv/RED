import uvicorn
from hardware import Arduino
from app import app
from hardware import arduino


if __name__ == "__main__":
    arduino.start(start_receiver=True)
    uvicorn.run(app)
    