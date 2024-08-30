import gymnasium as gym
from gymnasium.envs.registration import register

register(
    id='cs-v0',
    entry_point = 'gymEnv.envs.csEnv:CSEnv',
    max_episode_steps=20000,
)
