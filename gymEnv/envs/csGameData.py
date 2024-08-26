
import time
import numpy as np
class csData():
    def __init__(self):
        self.xPos = 0
        self.yPos = 0
        self.zPos = 0
        self.prexX = 0
        self.prevY = 0
        self.prevZ = 0


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
    
    #DEFINE REWARD TO BE A LINEAR FUNCTION OF DISTANCE THE PLAYER TRAVELED TOWARDS THE END, AND SPEED/VELOCITY. PUNISH IF FALLS 
    def evaluate(self):
        if(self.isTermCollision()):
            return -1000
        if(self.isCheeseCollision()):
            return 1000
        if(abs((10 - self.prevX) + (10 - self.prevY)) > abs((10 - self.xPos) + (10 - self.yPos))):
            #print("hereA")
            return 0.2
        return -2
        
    #IF SENT BACK TO BEGINING OR AT END?
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
        
    #CHANGE TO RETURN RELEVANT METADATA
    def observe(self):
        return [self.xPos, self.yPos, 10, 10]

    

    #UPDATE TO USE CS INTERACTION HANDLER
    def updatePlayer(self, action):

        #SET PREV VALUES
        self.prevX = self.xPos
        self.prevY = self.yPos

        #SELECT IN GAME MOVE
        if action == 0: #go right
            self.xPos += 0.5
        elif action == 1: #go left
            self.xPos -= 0.5
        elif action == 2: #go up
            self.yPos += 0.5
        else:
            self.yPos -= 0.5

        #RECORD THE NEW STATE OF THE GAME 

        
    
    #IF SENT BACK TO BEGINING
    def isTermCollision(self):
        if((self.xPos + 1 >= self.wallXLeft and self.xPos <= self.wallXRight) and (self.yPos + 1 >= self.wallYBottom and self.yPos <= self.wallYTop)):
            #print(f"{self.xPos}" + " " + f"{self.yPos}")
            return True
        elif((self.xPos < -20 or self.xPos > 20) or (self.yPos < -20 or self.yPos > 20)):
            return True
        else:
            return False
    
    #IF REACHED THE END ABOVE CERTAIN Z VALUE
    def isCheeseCollision(self):
        #print(f"{self.xPos}" + " " + f"{self.yPos}")
        if(self.xPos == 10 and self.yPos == 10):
            print("WINNER")
            #time.sleep(1)
            return True
        return False
        