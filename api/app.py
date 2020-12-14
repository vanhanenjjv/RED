from fastapi import Request, Form
from fastapi.responses import HTMLResponse
from fastapi.templating import Jinja2Templates
from fastapi.applications import FastAPI
from fastapi.staticfiles import StaticFiles
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
from state import state
from hardware import arduino


app = FastAPI()

origins = [
    "http://localhost:3000"
]

app.add_middleware(
    CORSMiddleware,
    allow_origins=origins,
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

app.mount("/static", StaticFiles(directory="static"), name="static")
templates = Jinja2Templates(directory="templates")

@app.get("/cards")
def get_state(): 
    return list(state.cards.values())

class RemoveCardModel(BaseModel):
    uid: str

@app.post("/remove-card")
def remove_card(card: RemoveCardModel):
    if card.uid in state.cards:
        state.cards.pop(card.uid)

    return

@app.post("/add-card")
def add_card():
    card = arduino.nfc.read_card()

    if card.uid not in state.cards:
        state.cards[card.uid] = card

    return
