from kivy.app import App

from main_layout import MainLayout

class SilokApp(App):
    def build(self):
        self.database = None
        
        return MainLayout()