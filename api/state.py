from typing import Union, Dict
from models import Card

class State:
    def __init__(self, cards: Union[Dict[int, Card], None]) -> None:
        self.cards: Dict[int, Card] = cards or {}

state = State(cards=None)
