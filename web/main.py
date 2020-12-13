import uvicorn
from arduino import arduino
from app import app
import message

if __name__ == "__main__":
    arduino.receive = message.parse
    arduino.start(start_receiver=True)

    uvicorn.run(app)
    