import random
import re
from collections import deque
from hmac import new
from types import new_class

import hydra
import numpy as np
import torch
from omegaconf import DictConfig, OmegaConf

from env import MouseEnv
from model import DQN
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
    env = MouseEnv(**config.env)
    model = DQN(env=env, **config.model)
