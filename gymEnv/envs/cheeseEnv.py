import gymnasium as gym
from gymnasium import spaces
import numpy as np 
from gymEnv.envs.csGameData import csGame



class Cheese2d(gym.Env):
    
    metadata = {"redner_modes": None}

    def __init__(self):
        self.game = csGame()
        self.action_space = spaces.Discrete(4)
        self.observation_space = spaces.Box(np.array([-20.0, -20.0, 9, 9]), np.array([20.0, 20.0, 11, 11]), (4,), np.float32)

    def reset(self, seed=None, options=None):

        del self.game
        self.game = cheeseGame()
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