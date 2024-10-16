from datetime import datetime

class Card:
    def __init__(self, date: datetime, content: str, type: str):
        self._date = date
        self._content = content
        self._type = type
    
    @property
    def date_str(self):
        return self._date.strftime("%Y%m%d_%H%M%S")
    
    @property
    def date(self):
        return self._date
    
    @property
    def content(self):
        return self._content
    
    @property
    def type(self):
        return self._type
    
    def __repr__(self):
        return f"[{self.date_str}][{self.type}]: {self.content}"
        
class CardList:
    def __init__(self):
        self.container = []
    
    def create(self, content:str, type:str):
        date = datetime.now()
        card = Card(date, content, type)
        
        self.container.append(card)

    def read(self, expr):
        cards = []
        for card in self.container:
            if expr(card):
                cards.append(card)
                
        return cards
    
    def update(self, card):
        pass
    
    def delete(self, card):
        try:
            index = self.container.index(card)
            self.container.pop(index)
        except ValueError:
            print(f"{card} not found in container.")
            