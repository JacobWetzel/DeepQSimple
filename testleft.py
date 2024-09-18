from pynput.keyboard import Key, Controller
import time


keyboard = Controller()
while True:

    keyboard.press(Key.right)
    keyboard.release(Key.right)
    time.sleep(.5)