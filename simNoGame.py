import random
import re
from collections import deque
from hmac import new
from types import new_class

import hydra
import numpy as np
import torch
from omegaconf import DictConfig, OmegaConf

from model import DQN, MouseEnv
from utils import AttrDict


# Initialize squares
@hydra.main(
    version_base=None,
    config_path="config",
    config_name="config",
)
def train(config: DictConfig):
    # config = OmegaConf.to_container(config, resolve=True)
    # config = AttrDict(config)
    env = MouseEnv(config.width, config.height, config.num_destinations)
    model = DQN(env=env, **config.model)

    for e in range(config.num_episodes):
        state = new_state(config.width, config.height)
        total_reward = 0
        running = True
        step = 0
        while running:
            action = model.choose_action(state, config.epsilon)
            direction = model.get_direction(action)
            cur_pos = state[0]

            new_pos = cur_pos + direction
            next_state = state.clone()
            next_state[0] = new_pos

            next_state = get_state(squares, current_square_index)
            reward = 0
            if isCol(squares[current_square_index]):
                reward = 1
                running = False

            remember(state, action, reward, next_state, not running)
            state = next_state
            total_reward += reward

            replay()

            if step % update_target_freq == 0:
                update_target_model()

            step += 1

        if epsilon > epsilon_min:
            epsilon *= epsilon_decay

        print(f"Episode: {e}/{num_episodes}, Total Reward: {total_reward}")
        print(f"Episode: {e}/{num_episodes}, Total Reward: {total_reward}")
        print(f"Episode: {e}/{num_episodes}, Total Reward: {total_reward}")
        print(f"Episode: {e}/{num_episodes}, Total Reward: {total_reward}")
        print(f"Episode: {e}/{num_episodes}, Total Reward: {total_reward}")
