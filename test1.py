import time
from threading import Thread
from datetime import datetime
from RPLCD.i2c import CharLCD
from inputs import get_gamepad

COLS = 16
ROWS = 2

lcd = CharLCD('PCF8574', 0x27, cols=COLS, rows=ROWS)

class Menu:
    def __init__(self):
        self.menu = {
            '$Rec': { 'Add Record': '' },  
            'Call': {}, 
            'SMS': {}, 
            'Mus': {}, 
            'AudBk': {}, 
            'Rss': {}
        }

        self.selection = 0
        self.menu_selection = ''

        self.update_display()

    def get_menu_options(self):
        output_menu = list(self.menu.keys())

        if self.menu_selection:
            output_menu = list(self.menu[self.menu_selection].keys())

        return output_menu

    def next(self):
        output_menu = self.get_menu_options()
        self.selection += 1
        if (self.selection >= len(output_menu)):
            self.selection = 0

        self.update_display()

    def back(self):
        output_menu = self.get_menu_options()
        self.selection -= 1
        if (self.selection < 0):
            self.selection = len(output_menu) - 1

        self.update_display()

    def go_home(self):
        self.menu_selection = ''
        self.update_display()

    def action(self):
        self.menu_selection = list(self.menu.keys())[self.selection]
        self.update_display()

    def update_display(self):
        output_menu = self.get_menu_options()
        output_menu.append("...")
        output_menu[self.selection] = f"[{output_menu[self.selection]}]"

        full_str = " ".join(output_menu)

        if (len(full_str) > COLS): 
            i = full_str.index('[')
            full_str = full_str[i:i+COLS-1] + ">"

        status_bar = time.strftime("%H:%M", time.localtime());
        status_bar += int(COLS - len(status_bar) - 3) * " "
        status_bar += "99%"

        lcd.clear()
        lcd.write_string(full_str + "\n\r" + status_bar)

    def status_refresh(self):
        while True:
            self.update_display()
            time.sleep(30)


def main():
    menu = Menu()

    status_thread = Thread(target=menu.status_refresh)
    status_thread.start()

    while True:
        events = get_gamepad()
        for event in events:
            if (event.state != 0):
                if (event.code == "ABS_HAT0X" and event.state == 1):
                    menu.next()
                if (event.code == "ABS_HAT0X" and event.state == -1):
                    menu.back()
                if (event.code == "BTN_SOUTH"):
                    menu.action()
                if (event.code == "BTN_EAST"):
                    menu.go_home()

    status_thread.join()        

main()
