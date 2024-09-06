
import time
import numpy as np
import os
from pynput.keyboard import Key, Controller

class csGame():
    def __init__(self):
        self.keyboard = Controller()
        script_dir = os.path.dirname(__file__)
        self.file_path = os.path.join(script_dir, '..', '..', 'nnValues.txt')
        self.timeStep = 0
        self.prevNumbers = [None] * 118
        self.numbers = self.read_filled_data()
        self.prevNumbers = self.numbers
        self.prevAction = -1
        #self.timeStep = 100
    def action(self, action):
        self.updatePlayer(action)
        return
    

    def read_filled_data(self, expected_length=118):
        """
        Reads data from a file and ensures that all expected_length spots are filled.
        
        :param file_path: Path to the file to read.
        :param expected_length: The number of values expected to be read from the file.
        :param retry_interval: Time to wait before retrying if the file doesn't have enough data.
        :return: A list of floats with the required number of elements.
        """
        start_time = time.time()
        while True:
            with open(self.file_path, 'r') as file:
                lines = file.readlines()
            # Convert each line to a float and store it in a list
            if(len(lines) > 2):
                numbers = [float(line.strip()) for line in lines]

                numbers.append(self.timeStep)
                # Check if the list has the required length
                if (len(numbers) == expected_length and ((numbers[:-1] != self.prevNumbers[:-1]) or (time.time() - start_time > 0.1))) :
                    # If we have more than expected, just take the first expected_length values
                    return numbers
                
                # If not enough data, wait for a short interval an pgwpgwphwpgpgw phwwpgwpgwpgph    pgwpgwpgwpgph wpgwpgwpgph wpgwpgwpgphpgphpgwpgwpgwphphpgwpgwpgwpgphpgphpgwpg pgwpgw phpgwphwpgwwd retry
            # print(f"Only found {len(numbers)} values, retrying...")

    #DEFINE REWARD TO BE A LINEAR FUNCTION OF DISTANCE THE PLAYER TRAVELED TOWARDS THE END, AND SPEED/VELOCITY. PUNISH IF FALLS 
    


    def evaluate(self):
        if(self.isTermCollision()):
            return -10000
        if(self.isCheeseCollision()):
            return 100000
        val1 = self.numbers[0] + 250
        #if(val1 < 0):
        #    val1 = val1 * 1.3
        val2 = 0
        self.timeStep += 1
        if(self.numbers[8] > 0):
            val2 = self.numbers[8]
        return (val1) + (val2) - (self.timeStep)
        
    #IF SENT BACK TO BEGINING OR AT END?
    def is_done(self):
        if((self.numbers[0] < -320 or self.numbers[2] < 123) or (self.numbers[0] >= 3808 and self.numbers[2] >= 123)):
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

        self.keyboard.release('g')
        self.keyboard.release('h')
        self.keyboard.release('w')
        #SELECT IN GAME MOVE
        if action == 0: #press Space
            self.keyboard.press(Key.space)
        elif action == 1: #walk forward
            self.keyboard.press('w')
        elif action == 2: #turn left
            if(self.prevAction != 2):
                self.keyboard.press('p')
                self.keyboard.release('p')
                self.keyboard.press('g')
        else:             #turn right
            if(self.prevAction != 3):
                self.keyboard.press('p')
                self.keyboard.release('p')
                self.keyboard.press('h')
        self.prevAction = action
        #RECORD THE NEW STATE OF THE GAME 

        
    
    #IF SENT BACK TO BEGINING
    def isTermCollision(self):
        if(self.numbers[2] < 123 or self.numbers[0] < -320): #REPLACE 500W ITH ACTUAL END VAL
            return True
        return False
    
    #IF REACHED THE END ABOVE CERTAIN Z VALUE
    def isCheeseCollision(self):
        if(self.numbers[0] >= 3808 and self.numbers[2] > 127): #REPLACE 1000 WITH ACTUAL END VAL
            return True
        return False
        