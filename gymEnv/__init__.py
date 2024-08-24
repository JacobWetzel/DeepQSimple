import gymnasium as gym
from gymnasium.envs.registration import register

register(
    id='cheese-v0',
    entry_point = 'gymEnv.envs.cheeseEnv:Cheese2d',
    max_episode_steps=400,
)
