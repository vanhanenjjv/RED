from typing import Union

class Card:
    def __init__(self, uid: str) -> None:
        self.uid = uid

class User:
    def __init__(self, name: str, card: Union[Card, None]) -> None:
        self.name = name
        self.card = card
