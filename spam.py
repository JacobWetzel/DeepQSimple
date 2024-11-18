import time
import numpy as np
import os
from pynput.keyboard import Key, Controller
import matplotlib.pyplot as plt

keyboard = Controller()

time.sleep(5)

for i in range (10000):

    keyboard.press('g')
    keyboard.release('g')
    keyboard.press('h')
    keyboard.release('h')