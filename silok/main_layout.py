import datetime

from kivy.uix.gridlayout import GridLayout
from kivy.uix.button import Button
from kivy.uix.label import Label
from kivy.uix.scrollview import ScrollView
from kivy.uix.textinput import TextInput
from kivy.uix.popup import Popup

from card_container import CardList

class ButtonLayout(GridLayout):
    def __init__(self, cards_view:"CardListView", *args, **kwargs):
        super().__init__(cols=3, *args, **kwargs)
        
        self.cards_view = cards_view
        
        brief_button = Button(text='Brief')
        brief_button.bind(on_release=lambda x: self.open_popup("Brief"))
        
        log_button = Button(text='Log')
        log_button.bind(on_release=lambda x: self.open_popup("Log"))
        
        review_button = Button(text='Review')
        review_button.bind(on_release=lambda x: self.open_popup("Review"))
        
        self.add_widget(brief_button)
        self.add_widget(log_button)
        self.add_widget(review_button)
    
    def open_popup(self, card_type: str):
        popup_content = GridLayout(cols=1)
        text_input = TextInput(hint_text="Enter new memo")
        save_button = Button(text="Save")
        popup_content.add_widget(text_input)
        popup_content.add_widget(save_button)

        popup = Popup(title="New Memo", content=popup_content, size_hint=(0.8, 0.5))
        save_button.bind(on_release=lambda x: self.save_memo(text_input.text, card_type, popup))
        popup.open()
    
    def save_memo(self, text, card_type, popup):
        if text:
            date = datetime.datetime.now()
            date_str = date.strftime("%Y%m%d_%H%M%S")
            self.cards_view.card_list.create(text, card_type)
            
            self.cards_view.populate_cards()
            
            popup.dismiss()

class CardListView(ScrollView):
    def __init__(self, card_list:CardList, *args, **kwargs):
        super().__init__(*args, **kwargs)
        
        self.card_list = card_list
        
        # GridLayout for vertical scrolling cards
        self.card_grid = GridLayout(cols=1, size_hint_y=None)
        self.card_grid.bind(minimum_height=self.card_grid.setter('height'))
        self.add_widget(self.card_grid)
        
    def populate_cards(self):
        self.card_grid.clear_widgets()
        
        
        cards = self.card_list.read(lambda x: True)
        for card in cards:
            self.add_card(card)

    def add_card(self, card):
        """ Adds a new card to the scroll view. """
        card_button = Button(text=f"{card}", size_hint_y=None, height=100)
        self.card_grid.add_widget(card_button)
        
class MainLayout(GridLayout):
    def __init__(self, card_list, *args, **kwargs):
        super().__init__(cols=1, *args, **kwargs)
        
        # Top Section: Date label
        self.add_widget(Label(text="2024-10-16", size_hint_y=None, height=50))
        
        # Middle Section: Scrollable Cards (Vertical Scroll)
        cards_view:CardListView = CardListView(card_list, size_hint_y=None, height=300)
        self.add_widget(cards_view)
        
        # Bottom Section: 3 buttons in grid layout
        self.add_widget(ButtonLayout(cards_view=cards_view, size_hint_y=None, height=50))
        
