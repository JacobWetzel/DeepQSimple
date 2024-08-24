
import time
import numpy as np
class cheeseGame():
    def __init__(self):
        self.xPos = -10 #1 thick up -10 -> -9
        self.yPos = -10 #1 thick right -10 -> -9
        self.cheeseX = 10 #1 thick up
        self.cheeseY = 10 #1 thick right
        self.wallXLeft = -1
        self.wallXRight = 1
        self.wallYTop = 5
        self.wallYBottom = -5
        self.numSteps = 0
        self.prevX = -10
        self.prevY = -10

    def action(self, action):
        self.updatePlayer(action)
        self.numSteps += 1
        return
    
    def evaluate(self):
        if(self.isTermCollision()):
            return -1000
        if(self.isCheeseCollision()):
            return 1000
        if(abs((10 - self.prevX) + (10 - self.prevY)) > abs((10 - self.xPos) + (10 - self.yPos))):
            #print("hereA")
            return 0.2
        return -2
        

    def is_done(self):
        if((self.xPos + 1 >= self.wallXLeft and self.xPos <= self.wallXRight) and (self.yPos + 1 >= self.wallYBottom and self.yPos <= self.wallYTop)):
            #print("here1")
            return True
        elif((self.xPos < -20 or self.xPos > 20) or (self.yPos < -20 or self.yPos > 20)):
            #print("here2")
            return True
        elif(self.xPos == 10 and self.yPos == 10):
            #print("here3")
            return True
        else:
            return False
        

    def observe(self):
        return [self.xPos, self.yPos, 10, 10]

    


    def updatePlayer(self, action):
        #print(self.xPos) 
        #print(self.yPos)

        #print("next")
        #print(f"{self.xPos}" + " " + f"{self.yPos}")
        rowStr = ""
        #print("\n")
        '''for i in range (-20, 20, 1):
            for j in range (-20, 20, 1):
                if i == 10 and j == 10:
                    rowStr += "C"
                elif i == self.yPos and j == self.xPos:
                    rowStr += "X"
                else:
                    rowStr += "="
           # print(rowStr)
            rowStr += "\n"
        print(rowStr)'''
        self.prevX = self.xPos
        self.prevY = self.yPos
        if action == 0: #go right
            self.xPos += 0.5
        elif action == 1: #go left
            self.xPos -= 0.5
        elif action == 2: #go up
            self.yPos += 0.5
        else:
            self.yPos -= 0.5
    
    def isTermCollision(self):
        if((self.xPos + 1 >= self.wallXLeft and self.xPos <= self.wallXRight) and (self.yPos + 1 >= self.wallYBottom and self.yPos <= self.wallYTop)):
            #print(f"{self.xPos}" + " " + f"{self.yPos}")
            return True
        elif((self.xPos < -20 or self.xPos > 20) or (self.yPos < -20 or self.yPos > 20)):
            return True
        else:
            return False
    
    def isCheeseCollision(self):
        #print(f"{self.xPos}" + " " + f"{self.yPos}")
        if(self.xPos == 10 and self.yPos == 10):
            print("WINNER")
            #time.sleep(1)
            return True
        return False
        