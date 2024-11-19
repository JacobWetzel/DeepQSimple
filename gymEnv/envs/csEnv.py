import gymnasium as gym
from gymnasium import spaces
import numpy as np 
from gymEnv.envs.csGameData import csGame
from pynput.keyboard import Key, Controller
import time
import random
import os
import getRandomSpawn


class CSEnv(gym.Env):
    
    
    metadata = {"redner_modes": None}

    def __init__(self):
        self.keyboard = Controller()
        self.keyboard.press('i')
        self.keyboard.release('i')
        script_dir = os.path.dirname(__file__)
        self.file_path = os.path.join(script_dir, '..', '..', 'nnValues.txt')
        self.game = csGame()
        self.action_space = spaces.Discrete(3)
        self.getRandBlock = getRandomSpawn.randBlocks()
        #self.action_space = spaces.Discrete(2)
        self.observation_space = spaces.Box(np.array([
        -10000.0, -10000.0,-10000.0, -10000.0, -10000.0, -10000.0,-10000.0, -10000.0, 
        -10000.0, -10000.0, -10000.0,-10000.0, -10000.0,
        -10000.0, -10000.0, -10000.0,-10000.0, -10000.0,
        -10000.0, -10000.0, -10000.0, -10000.0,-10000.0, -10000.0, -10000.0, -10000.0,
        -10000.0, 
        -10000.0, -10000.0, -10000.0, -10000.0,-10000.0, -10000.0, -10000.0, -10000.0,
        -10000.0, -10000.0, -10000.0, -10000.0,-10000.0, -10000.0, -10000.0, -10000.0, -10000.0
        -10000.0, -10000.0, -10000.0, -10000.0,-10000.0, -10000.0, -10000.0, -10000.0, -10000.0
        -10000.0, -10000.0, -10000.0, -10000.0,-10000.0, -10000.0, -20000.0, -600.0
         ]), 
         np.array([
         10000.0, 
         10000.0, 10000.0,10000.0, 10000.0, 10000.0, 10000.0,
         10000.0,10000.0, 10000.0, 10000.0, 10000.0,
         10000.0,10000.0, 10000.0, 10000.0, 10000.0,
         10000.0, 10000.0,10000.0, 10000.0, 10000.0, 10000.0,
         10000.0, 10000.0,10000.0, 10000.0, 10000.0, 10000.0,
        10000.0, 10000.0,10000.0, 10000.0, 10000.0, 10000.0, 10000.0, 10000.0,
         10000.0, 10000.0,10000.0, 10000.0, 10000.0, 10000.0, 10000.0,
         10000.0, 10000.0,10000.0, 10000.0, 10000.0, 10000.0, 10000.0, 10000.0,
         10000.0, 10000.0,10000.0, 10000.0, 10000.0, 20000.0, 10000.0
         ]), (59,), np.float32)
    def reset(self, seed=None, options=None):
        del self.game
        self.keyboard.press('p')
        self.keyboard.release('p')
        self.keyboard.press('p')
        self.keyboard.release('p')
        self.keyboard.press('p')
        self.keyboard.release('p')
        all_keys = [
            'd', 'e', 'f', 'g', 'h', 'i', 'o', 'p', 'w']
        self.keyboard.release(Key.space)
        self.keyboard.press(Key.right)
        self.keyboard.release(Key.right)
        for k in all_keys:
            self.keyboard.release(k)
        time.sleep(0.55)
        rInt = random.randint(0, 5)
        
        newSpawn = self.getRandBlock.getRandomSpawn()
        
        self.keyboard.press('~')
        self.keyboard.release('~')
        for ch in newSpawn:
            self.keyboard.press(ch)
            self.keyboard.release(ch)
        
        self.keyboard.press(Key.enter)
        self.keyboard.release(Key.enter)
        
        self.keyboard.press('~')
        self.keyboard.release('~')
        
        
        #if rInt == 0:
        if rInt == 0:
            self.keyboard.press('i')
            time.sleep(0.85)
            self.keyboard.release('i')
        elif rInt == 1:
            self.keyboard.press('6')
            time.sleep(0.85)
            self.keyboard.release('6')
        elif rInt == 2:
            self.keyboard.press('7')
            time.sleep(0.85)
            self.keyboard.release('7')
        elif rInt == 3:
            self.keyboard.press('8')
            time.sleep(0.85)
            self.keyboard.release('8')
        elif rInt == 4:
            self.keyboard.press('9')
            time.sleep(0.85)
            self.keyboard.release('9')
        else:
            self.keyboard.press('0')
            time.sleep(0.85)
            self.keyboard.release('0')
        with open(self.file_path, 'w') as file1:
            file1.write(' ')
        self.game = csGame()
        obs = self.game.observe()
        return obs

    def step(self, action):
        self.game.action(action)
        obs = self.game.observe()
        reward = self.game.evaluate()
        done = self.game.is_done()
        #print(done)
        return obs, reward, done, done, {}
    
    def render(self):
        return