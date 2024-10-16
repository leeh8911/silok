from kivy.app import App

from main_layout import MainLayout
from card_container import CardList

class SilokApp(App):
    def build(self):
        self.card_list = CardList()
        
        return MainLayout(self.card_list)