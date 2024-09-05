from pynput import keyboard
import time

# Global variable to control the loop
stop_loop = False

# Function to handle key press events
def on_press(key):
    global stop_loop
    try:
        if key == keyboard.Key.up:
            print("Up arrow key pressed! Stopping loop...")
            stop_loop = True  # Set the variable to True to stop the loop
            return False  # Stop the listener
    except AttributeError:
        pass

# Function that runs an infinite loop
def infinite_loop():
    while not stop_loop:  # Continue looping until stop_loop is True
        print("Loop is running...")
        time.sleep(1)  # Simulate work with a 1-second delay

# Start the keyboard listener in the background
listener = keyboard.Listener(on_press=on_press)
listener.start()

# Run the infinite loop
infinite_loop()

# Wait for the listener to finish (optional)
listener.join()