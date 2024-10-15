from kivy.app import App

from main_layout import MainLayout

class SilokApp(App):
    def build(self):
        self.card_list = {}
        
        return MainLayout(self.card_list)