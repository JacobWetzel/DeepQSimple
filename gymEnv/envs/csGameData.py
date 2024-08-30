
import time
import numpy as np
from pynput.keyboard import Key, Controller

class csGame():
    def __init__(self):
        self.keyboard = Controller()

        #self.arr(117,) = 
        with open('data.txt', 'r') as file:  # Replace 'data.txt' with your file name
        # Step 2: Read all lines and strip any newline characters
            lines = file.readlines()
        # Step 3: Convert each line to a float and store in a list
        self.numbers = [float(line.strip()) for line in lines]
        self.prevNumbers = self.numbers

    def action(self, action):
        self.updatePlayer(action)
        self.numSteps += 1
        return
    
    #DEFINE REWARD TO BE A LINEAR FUNCTION OF DISTANCE THE PLAYER TRAVELED TOWARDS THE END, AND SPEED/VELOCITY. PUNISH IF FALLS 
    def evaluate(self):
        if(self.isTermCollision()):
            return -1000
        if(self.isCheeseCollision()):
            return 100000
        return (self.numbers[0] - self.prevNumbers[0]) + (self.numbers[8])
        
    #IF SENT BACK TO BEGINING OR AT END?
    def is_done(self):
        if((self.numbers[0] < -500 or self.numbers[2] < 128) or self.numbers[0] > 5000):
            return True
        elif((self.xPos < -20 or self.xPos > 20) or (self.yPos < -20 or self.yPos > 20)):
            #print("here2")
            return True
        elif(self.xPos == 10 and self.yPos == 10):
            #print("here3")
            return True
        else:
            return False
        
    #CHANGE TO RETURN RELEVANT METADATA
    def observe(self):
        self.prevNumbers = self.numbers
        with open('data.txt', 'r') as file:  # Replace 'data.txt' with your file name
            lines = file.readlines()
        # Step 3: Convert each line to a float and store in a list
        numbers = [float(line.strip()) for line in lines]

        return numbers

    

    #UPDATE TO USE CS INTERACTION HANDLER
    def updatePlayer(self, action):

        self.keyboard.release('g')
        self.keyboard.release('h')
        self.keyboard.release('w')
        #SELECT IN GAME MOVE
        if action == 0: #press Space
            self.keyboard.press('space')
        elif action == 1: #walk forward
            self.keyboard.press('w')
        elif action == 2: #turn left
            self.keyboard.press('p')
            self.keyboard.release('p')
            self.keyboard.press('g')
        else:             #turn right
            self.keyboard.press('p')
            self.keyboard.release('p')
            self.keyboard.press('h')

        #RECORD THE NEW STATE OF THE GAME 

        
    
    #IF SENT BACK TO BEGINING
    def isTermCollision(self):
        if(self.numbers[2] < 128 or self.numbers[0] < -500): #REPLACE 500W ITH ACTUAL END VAL
            return True
        return False
    
    #IF REACHED THE END ABOVE CERTAIN Z VALUE
    def isCheeseCollision(self):
        if(self.numbers[0] > 5000 and self.numbers[2] > 128): #REPLACE 1000 WITH ACTUAL END VAL
            return True
        return False
        