import datetime

from kivy.uix.gridlayout import GridLayout
from kivy.uix.button import Button
from kivy.uix.label import Label
from kivy.uix.scrollview import ScrollView
from kivy.uix.textinput import TextInput
from kivy.uix.popup import Popup

class ButtonLayout(GridLayout):
    def __init__(self, card_list_ref, *args, **kwargs):
        super().__init__(cols=3, *args, **kwargs)
        
        self.card_list_ref = card_list_ref
        
        write_button = Button(text='Write')
        write_button.bind(on_release=lambda x: self.open_popup())
        
        self.add_widget(write_button)
        self.add_widget(Button(text="Edit"))
        self.add_widget(Button(text="Delete"))
    
    def open_popup(self):
        popup_content = GridLayout(cols=1)
        text_input = TextInput(hint_text="Enter new memo")
        save_button = Button(text="Save")
        popup_content.add_widget(text_input)
        popup_content.add_widget(save_button)

        popup = Popup(title="New Memo", content=popup_content, size_hint=(0.8, 0.5))
        save_button.bind(on_release=lambda x: self.save_memo(text_input.text, popup))
        popup.open()
    
    def save_memo(self, text, popup):
        if text:
            date = datetime.datetime.now()
            date_str = date.strftime("%Y%m%d_%H%M%S")
            self.card_list_ref.card_list[date_str] = text
            
            self.card_list_ref.populate_cards()
            
            popup.dismiss()

class CardListView(ScrollView):
    def __init__(self, card_list, *args, **kwargs):
        super().__init__(*args, **kwargs)
        
        self.card_list = card_list
        
        # GridLayout for vertical scrolling cards
        self.card_grid = GridLayout(cols=1, size_hint_y=None)
        self.card_grid.bind(minimum_height=self.card_grid.setter('height'))
        self.add_widget(self.card_grid)
        
    def populate_cards(self):
        self.card_grid.clear_widgets()
        sorted_entries = sorted(self.card_list.items(), key=lambda x: x[0], reverse=True)
        for key, entry in sorted_entries:
            self.add_card(key, entry)

    def add_card(self, key, text):
        """ Adds a new card to the scroll view. """
        card_button = Button(text=text, size_hint_y=None, height=100)
        self.card_grid.add_widget(card_button)
        
class MainLayout(GridLayout):
    def __init__(self, card_list, *args, **kwargs):
        super().__init__(cols=1, *args, **kwargs)
        
        # Top Section: Date label
        self.add_widget(Label(text="2024-10-16", size_hint_y=None, height=50))
        
        # Middle Section: Scrollable Cards (Vertical Scroll)
        cards_view = CardlistView(card_list, size_hint_y=None, height=300)
        self.add_widget(cards_view)
        
        # Bottom Section: 3 buttons in grid layout
        self.add_widget(ButtonLayout(card_list_ref=cards_view, size_hint_y=None, height=50))
        
