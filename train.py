from collections import deque
from pathlib import Path

import torch
import wandb
import yaml
from addict import Dict as AttrDict
from omegaconf import DictConfig, OmegaConf

from model import DQN
from utils import get_optimizer

from .env import MouseEnv


def train(config: DictConfig):
    config = OmegaConf.to_container(config, resolve=True)
    print(yaml.dump(config))
    config = AttrDict(config)
    env = MouseEnv(**config.env)
    dqn = DQN(env=env, **config.model)

    optimizer = get_optimizer(
        config.optimizer.fn, dqn.model.parameters(), config.optimizer.kwargs
    )

    if config.criterion == "mse":
        criterion = torch.nn.MSELoss(**config.criterion.kwargs)
    else:
        raise ValueError(f"Criterion {config.criterion} not supported")

    memory = deque(maxlen=config.memory_size)

    if save_dir is not None:
        save_dir = Path(save_dir)
        save_dir.mkdir(parents=True, exist_ok=True)

    self.model.train()
    for e in range(num_episodes):
        print(f"Episode {e}/{num_episodes}")
        state, _ = self.env.reset()
        total_reward = 0
        running = True
        step = 0
        while running:
            action = self.choose_action(state)
            next_state, reward, terminated, truncated, info = self.env.step(action)
            self.remember(state, action, reward, next_state, terminated)
            self.train_step()
            if step % self.update_target_freq == 0:
                self.update_target_model()
            step += 1
            total_reward += reward
            state = next_state
            if terminated or step >= max_steps:
                running = False

        print(f"Total reward: {total_reward}")
        if log_freq is not None and e % log_freq == 0:
            wandb.log({"total_reward": total_reward})

        if e % save_freq == 0:
            torch.save(
                {
                    "model": self.model.state_dict(),
                    "optimizer": self.optimizer.state_dict(),
                    "epsilon": self.epsilon,
                    "episode": e,
                },
                save_dir / f"model_{e}.pt",
            )
        if self.epsilon > self.epsilon_min:
            self.epsilon *= self.epsilon_decay
            self.epsilon *= self.epsilon_decay
