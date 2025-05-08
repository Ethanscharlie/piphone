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
        self.menu = [
            'Call', 
            'SMS', 
            'Mus', 
            'AudBk', 
            '$Rec', 
            'Rss'
        ]

        self.selection = 0

        self.update_display()

    def next(self):
        self.selection += 1
        if (self.selection >= len(self.menu)):
            self.selection = 0

        self.update_display()

    def back(self):
        self.selection -= 1
        if (self.selection < 0):
            self.selection = len(self.menu) - 1

        self.update_display()

    def update_display(self):
        output_menu = self.menu.copy()
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
            if (event.state):
                if (event.code == "BTN_EAST"):
                    menu.next()
                elif (event.code == "BTN_NORTH"):
                    menu.back()

    status_thread.join()        

main()
