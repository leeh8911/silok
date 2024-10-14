from kivy.uix.gridlayout import GridLayout
from kivy.uix.button import Button
from kivy.uix.slider import Slider
from kivy.uix.label import Label
from kivy.uix.popup import Popup
from kivy.uix.scrollview import ScrollView
from kivy.uix.widget import Widget

class ButtonLayout(GridLayout):
    def __init__(self, *args, **kwargs):
        super().__init__(cols=3, *args, **kwargs)
        
        write_button = Button(text='write')
        write_button.bind(on_release=lambda x: print("Write button click"))
        
        self.add_widget(write_button)
        self.add_widget(Button(text="2"))
        self.add_widget(Button(text="3"))
        
class CardViewer(Widget):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.cards = []
        
    def add_card(self, card:dict):
        self.cards.append(card)
        
    def pop_front(self):
        self.cards = self.cards[1:]
    
class MainLayout(GridLayout):
    def __init__(self, *args, **kwargs):
        super().__init__(cols=1, *args, **kwargs)
        
        scroll_view = ScrollView()
        scroll_view.add_widget(Slider())
        
        self.add_widget(Button(text="date"))
        self.add_widget(scroll_view)
        self.add_widget(ButtonLayout())