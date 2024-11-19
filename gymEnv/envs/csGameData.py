
import time
import numpy as np
import os
from pynput.keyboard import Key, Controller
import matplotlib.pyplot as plt
import getRandomSpawn

# Enable interactive mode
plt.ion()

# Create a figure and axis
fig, ax = plt.subplots()

# Initialize variables to store points and track calls
x_vals = []
y_vals = []
call_count = 0  # To track the number of times the function is called

def plot_point(x, y):
    global x_vals, y_vals, call_count
    x_vals.append(x)
    y_vals.append(y)
    call_count += 1

    # Normalize call_count to a value between 0 and 1
    max_calls = 50  # Adjust as needed
    norm = min(call_count / max_calls, 1)

    # Calculate color transitioning from blue to red
    r = norm  # Red value increases over time
    g = 0     # Green remains constant
    b = 1 - norm  # Blue value decreases over time
    color = (r, g, b)

    # Plot the point
    ax.scatter(x, y, color=color)

    # Update the plot
    plt.draw()
    plt.pause(0.01)  # Pause to allow the plot to update



class csGame():
    def __init__(self):
        self.first = True
        self.keyboard = Controller()
        self.randB = getRandomSpawn.randBlocks()
        script_dir = os.path.dirname(__file__)
        self.file_path = os.path.join(script_dir, '..', '..', 'nnValues.txt')
        self.i = 0
        self.timeStep = 0
        self.farthestXPos = 24
        self.goalSpot = self.randB.getRandomGoal
        self.prevNumbers = [None] * 59
        self.isReset = True
        self.numbers = self.read_filled_data()
        self.prevNumbers = self.numbers
        self.prevAction = -1
        self.maxSpeed=50
        self.first = False
        self.keyboard.press(Key.right)
        self.keyboard.release(Key.right)

    def action(self, action):
        self.updatePlayer(action)
        return
    

    def read_filled_data(self, expected_length=59):
        """
        Reads data from a file and ensures that all expected_length spots are filled.
        
        :param file_path: Path to the file to read.
        :param expected_length: The number of values expected to be read from the file.
        :param retry_interval: Time to wait before retrying if the file doesn't have enough data.
        :return: A list of floats with the required number of elements.
        """
        start_time = time.time()
        while True:
            if(os.path.getsize(self.file_path) > 4):

                try:
                    #print("a")
                    with open(self.file_path, 'r') as file:
                        lines = file.readlines()
                    #print("b")
                    # Convert each line to a float and store it in a list
                    if(len(lines) == 57):
                        numbers = [float(line.strip()) for line in lines]

                        numbers.append(self.timeStep)
                        if self.first:
                            self.farthestXPos = numbers[0]
                        numbers.append(self.farthestXPos)
                        # Check if the list has the required length
                        if (len(numbers) == expected_length and ((numbers[:-2] != self.prevNumbers[:-2]) or (time.time() - start_time > 0.2))) :
                            # If we have more than expected, just take the first expected_length values
                            if(len(numbers) == 0):
                                return [0] * 59
                            return numbers
                except Exception as e:
                    print(e)
                    return [0] * 59
                # If not enough data, wait for a short interval an pgwpgwphwpgpgw phwwpgwpgwpgph    pgwpgwpgwpgph wpgwpgwpgph wpgwpgwpgphpgphpgwpgwpgwphphpgwpgwpgwpgphpgphpgwpg pgwpgw phpgwphwpgwwd retry
            else:
                time.sleep(0.01)
            # print(f"Only found {len(numbers)} values, retrying...")

    #DEFINE REWARD TO BE A LINEAR FUNCTION OF DISTANCE THE PLAYER TRAVELED TOWARDS THE END, AND SPEED/VELOCITY. PUNISH IF FALLS 
    


    def evaluate(self):
        if(self.isTermCollisionA()):
            plot_point(self.numbers[0], self.numbers[1])
            return -100
        if(self.isTermCollisionB()):
            plot_point(self.numbers[0], self.numbers[1])
            return -50
        if(self.isCheeseCollision()):
            plot_point(self.numbers[0], self.numbers[1])
            return 100
        '''        val1 = 0
                if(self.numbers[0] > self.farthestXPos):
                    self.farthestXPos = self.numbers[0]
                    val1 += (self.numbers[0] - self.prevNumbers[0]) ** (self.numbers[4]/self.maxSpeed)

                #if(val1 < 0):
                #    val1 = val1 * 1.3
                val2 = 0
                
                if(self.numbers[8] > 0 and self.numbers[4]):
                    tmp = max(self.numbers[8], 100)
                    val2 = (tmp ** (self.numbers[4] / self.maxSpeed)) * 0.01
                val3 = 0
                if(self.numbers[4] > 0 and self.numbers[8] > 0):
                    val3 = (abs(self.numbers[9]) * 0.1)
                print(val1, val2, val3, self.timeStep, (val1) + (val2) + val3 - (self.timeStep))
                val4 = 0
                if(self.numbers[46] == 1):
                    val4 = 40000
                if(self.maxSpeed < self.numbers[4]):
                    self.maxSpeed = self.numbers[4]'''
        
        val1 = val2 = val3 = 0
        
        #change in euclidean distance from goal spot
        euclidDistChange = (self.goalSpot[0][0] - self.prevNumbers[0]) - (self.goalSpot[0][0] - self.numbers[0])
        euclidDistChange += (self.goalSpot[0][1] - self.prevNumbers[1]) - (self.goalSpot[0][1] - self.numbers[1])
        
        
        if(self.numbers[0] > self.farthestXPos):
            val1 = self.numbers[0] - self.prevNumbers[0]
            self.farthestXPos = self.numbers[0]
        #if(self.numbers[56] == 1):
        #    val3 = 10
        #    print("hererererererer")
        if(self.i == 5):
            self.timeStep += 0.02
            self.i = 0
        self.i += 1
        return (euclidDistChange) - self.timeStep#+ val3 + val4 - (self.timeStep)
        
    #IF SENT BACK TO BEGINING OR AT END?
    def is_done(self):
        if((self.isTermCollisionA() or self.isTermCollisionB() or self.isCheeseCollision())):
            print("is it here???")
            return True
        else:
            return False
        
    #CHANGE TO RETURN RELEVANT METADATA
    def observe(self):
        self.prevNumbers = self.numbers
        self.numbers = self.read_filled_data()

        return self.numbers

    

    #UPDATE TO USE CS INTERACTION HANDLER
    def updatePlayer(self, action):
        
        self.keyboard.release('w')
        self.keyboard.press(Key.space)
        #SELECT IN GAME MOVE
        if action == 0: #press Space
            self.keyboard.press('p')
            self.keyboard.release('p')
            time.sleep(0.01)
            self.keyboard.press(Key.space)
        elif action == 1: #turn left
            print("left")
            if(self.prevAction != 1):
                self.keyboard.press('p')
                self.keyboard.release('p')
                time.sleep(0.01)
                self.keyboard.press('g')
                self.keyboard.release('g')
        else:             #turn right
            print("right")
            if(self.prevAction != 2):
                self.keyboard.press('p')
                self.keyboard.release('p')
                time.sleep(0.01)
                self.keyboard.press('h')
                self.keyboard.release('h')
        self.prevAction = action
        self.keyboard.press(Key.right)
        self.keyboard.release(Key.right)
        cur = time.time()
        nCur = time.time()
        while(nCur - cur < 0.025):
            nCur = time.time()
        #print(nCur - cur)
        self.keyboard.press(Key.left)
        self.keyboard.release(Key.left)
        with open(self.file_path, 'w') as file1:
            file1.write('')  # Overwrite the file with an empty string
        #time.sleep(0.1)
        
        #RECORD THE NEW STATE OF THE GAME 

        
    
    #IF SENT BACK TO BEGINING
    def isTermCollisionA(self):
    #    if(self.numbers[0] < -0 or (self.numbers[0] < 192 and (self.numbers[1] > 400 or self.numbers[1] < -384))): #REPLACE 500W ITH ACTUAL END VAL
    #        print("here?\n")
    #        return True
        return False
    def isTermCollisionB(self):
        if(self.numbers[2] < 123): #REPLACE 500W ITH ACTUAL END VAL
            print("is this")
            return True
        return False
    #IF REACHED THE END ABOVE CERTAIN Z VALUE
    def isCheeseCollision(self):
        x1 = self.goalSpot[0][0] - (self.goalSpot[1][0]/2)
        x2 = self.goalSpot[0][0] + (self.goalSpot[1][0]/2)
        y1 = self.goalSpot[0][1] - (self.goalSpot[1][1]/2)
        y2 = self.goalSpot[0][1] + (self.goalSpot[1][1]/2)
        inX = (self.numbers[0] >= x1 and self.numbers[0] <= x2)
        inY = (self.numbers[1] >= y1 and self.numbers[1] <= y2)
        if(inX and inY and self.numbers[2] > 127): #REPLACE 1000 WITH ACTUAL END VAL
            print("no way here")
            return True
        return False
        