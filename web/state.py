from typing import Union, List
from models import User, Card

class State:
    def __init__(self, users: Union[List[User], None]) -> None:
        self.current_card: Union[Card, None] = None
        self._users      : List[User]        = users or []

    @property
    def users(self) -> List[User]:
        return self._users

    @property
    def current_card(self) -> Union[Card, None]:
        return self._current_card

    @current_card.setter
    def current_card(self, card: Card) -> None:
        self._current_card = card

state = State(users=None)
