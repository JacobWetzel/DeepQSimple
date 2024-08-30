import pyautogui
import time

def move_mouse():
    time.sleep(5)
    for _ in range(10000):  # Repeat the movement 10 times
        # Get the current mouse position
        x, y = pyautogui.position()
        
        # Move the mouse to the right
        pyautogui.moveTo(x + 10, y, duration=0.2)
        
        # Move the mouse down
        pyautogui.moveTo(x + 10, y + 10, duration=0.2)
        
        # Move the mouse to the left
        pyautogui.moveTo(x, y + 10, duration=0.2)
        
        # Move the mouse up
        pyautogui.moveTo(x, y, duration=0.2)
        
        # Pause for a bit before the next iteration
        time.sleep(0.5)
       
def press_l():
    time.sleep(5)
    for i in range (0, 1000, 1):
        pyautogui.write('l')


if __name__ == "__main__":
    move_mouse()
