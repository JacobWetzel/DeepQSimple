import gymnasium as gym
from gymnasium import spaces
import numpy as np 
from gymEnv.envs.csGameData import csGame
from pynput.keyboard import Key, Controller
import time



class CSEnv(gym.Env):
    
    
    metadata = {"redner_modes": None}

    def __init__(self):
        self.keyboard = Controller()
        self.keyboard.press('i')
        self.keyboard.release('i')
        self.game = csGame()
        self.action_space = spaces.Discrete(4)
        self.observation_space = spaces.Box(np.array([-10000.0, -10000.0, -10000.0, -10000.0,-10000.0, -10000.0, -10000.0, -10000.0,
         -10000.0, -10000.0, -10000.0, -10000.0,-10000.0, -10000.0, -10000.0, -10000.0,-10000.0, -10000.0, -10000.0, -10000.0,
         -10000.0, -10000.0, -10000.0, -10000.0,-10000.0, -10000.0, -10000.0, -10000.0,-10000.0, -10000.0, -10000.0, -10000.0,
         -10000.0, -10000.0, -10000.0, -10000.0,-10000.0, -10000.0, -10000.0, -10000.0,
         -10000.0, -10000.0, -10000.0, -10000.0,-10000.0, -10000.0, -10000.0, -10000.0,
         -10000.0, -10000.0, -10000.0, -10000.0,-10000.0, -10000.0, -10000.0, -10000.0,-10000.0, -10000.0, -10000.0, -10000.0,
         -10000.0, -10000.0, -10000.0, -10000.0,-10000.0, -10000.0, -10000.0, -10000.0,-10000.0, -10000.0, -10000.0, -10000.0,
         -10000.0, -10000.0, -10000.0, -10000.0,-10000.0, -10000.0, -10000.0, -10000.0,
         -10000.0, -10000.0, -10000.0, -10000.0,-10000.0, -10000.0, -10000.0, -10000.0,
         -10000.0, -10000.0, -10000.0, -10000.0,-10000.0, -10000.0, -10000.0, -10000.0,-10000.0, -10000.0, -10000.0, -10000.0,
         -10000.0, -10000.0, -10000.0, -10000.0,-10000.0, -10000.0, -10000.0, -10000.0,-10000.0, -10000.0, -10000.0, -10000.0,
         -10000.0, -10000.0, -10000.0, -10000.0,-10000.0]), np.array([10000.0, 10000.0, 10000.0, 10000.0,
         10000.0, 10000.0,10000.0, 10000.0, 10000.0, 10000.0,
         10000.0, 10000.0,10000.0, 10000.0, 10000.0, 10000.0,
         10000.0, 10000.0,10000.0, 10000.0, 10000.0, 10000.0,
         10000.0, 10000.0,10000.0, 10000.0, 10000.0, 10000.0,
         10000.0, 10000.0,10000.0, 10000.0, 10000.0, 10000.0,
         10000.0, 10000.0,10000.0, 10000.0, 10000.0, 10000.0,
         10000.0, 10000.0,10000.0, 10000.0, 10000.0, 10000.0,
         10000.0, 10000.0,10000.0, 10000.0, 10000.0, 10000.0,
         10000.0, 10000.0,10000.0, 10000.0, 10000.0, 10000.0,
         10000.0, 10000.0,10000.0, 10000.0, 10000.0, 10000.0,
         10000.0, 10000.0,10000.0, 10000.0, 10000.0, 10000.0,
         10000.0, 10000.0,10000.0, 10000.0, 10000.0, 10000.0,
         10000.0, 10000.0,10000.0, 10000.0, 10000.0, 10000.0,
         10000.0, 10000.0,10000.0, 10000.0, 10000.0, 10000.0,
         10000.0, 10000.0,10000.0, 10000.0, 10000.0, 10000.0,
         10000.0, 10000.0,10000.0, 10000.0, 10000.0, 10000.0,
         10000.0, 10000.0,10000.0, 10000.0, 10000.0, 10000.0,
         10000.0, 10000.0,10000.0, 10000.0, 10000.0, 10000.0,
         10000.0, 10000.0, 10000.0, 10000.0, 10000.0]), (117,), np.float32)
    def reset(self, seed=None, options=None):

        self.keyboard.press('p')
        self.keyboard.release('p')
        self.keyboard.press('p')
        self.keyboard.release('p')
        self.keyboard.press('p')
        self.keyboard.release('p')
        all_keys = [
            'd', 'e', 'f', 'g', 'h', 'i', 'o', 'p', 'w']
        self.keyboard.release(Key.space)
        for k in all_keys:
            self.keyboard.release(k)
        time.sleep(1)
        self.keyboard.press('i')
        time.sleep(1)
        self.keyboard.release('i')
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