from fastapi import Request, Form
from fastapi.responses import HTMLResponse
from fastapi.templating import Jinja2Templates
from fastapi.applications import FastAPI
from fastapi.staticfiles import StaticFiles
from state import state

app = FastAPI()
app.mount("/static", StaticFiles(directory="static"), name="static")
templates = Jinja2Templates(directory="templates")

@app.get("/state")
def get_state(): 
    return {
        "users": state.users
    }

