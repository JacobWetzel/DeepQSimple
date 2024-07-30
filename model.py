import random
from collections import deque
from math import prod
from pathlib import Path
from typing import Optional

import torch
import wandb
from gymnasium.spaces.utils import flatdim
from torch import nn


class MLP(nn.Module):
    def __init__(self, obs_size, num_actions):
        super().__init__()

        self.num_actions = num_actions
        self.fc1 = nn.Linear(obs_size, 128)
        self.fc2 = nn.Linear(128, 128)
        self.fc3 = nn.Linear(128, num_actions)

    def forward(self, x):
        x = torch.relu(self.fc1(x))
        x = torch.relu(self.fc2(x))
        x = self.fc3(x)
        return x


class CNN(nn.Module):
    def __init__(self, in_channels, num_actions, hidden_dims, fc_dim):
        super().__init__()

        self.convs = nn.Sequential()
        for hidden_dim in hidden_dims:
            self.convs.append(nn.Conv2d(in_channels, hidden_dim, 3, padding=1))
            self.convs.append(nn.ReLU())
            in_channels = hidden_dim
        self.pool = nn.AdaptiveAvgPool2d((1, 1))
        self.out = nn.Sequential(
            nn.Linear(hidden_dims[-1], fc_dim),
            nn.ReLU(),
            nn.Linear(fc_dim, num_actions),
        )

    def forward(self, x):
        x = self.convs(x)
        x = self.pool(x)
        x = self.out(x)
        return x


class DQNAgent:
    def __init__(
        self,
        env,
        device: str = "cpu",
    ):
        self.env = env
        self.device = device
        self.observation_space = flatdim(env.observation_space)
        self.action_space = flatdim(env.action_space)

        self.model = MLP(
            prod(env.observation_space.shape), prod(env.action_space.shape)
        ).to(device)
        self.target_model = MLP(
            prod(env.observation_space.shape), prod(env.action_space.shape)
        ).to(device)

        self.update_target_model()

    def choose_action(self, state, epsilon):
        if torch.randn() < epsilon:
            return torch.randint(0, self.num_actions, state.shape[0])
        else:
            q_values = self.model(state)
            return torch.argmax(q_values[0], dim=1)

    def remember(self, state, action, reward, next_state, done):
        self.memory.append((state.flatten(), action, reward, next_state, done))

    def train_step(self):
        if len(self.memory) < self.batch_size:
            return

        states, actions, rewards, next_states, dones = zip(
            *random.sample(self.memory, self.batch_size)
        )

        states = torch.stack(states)
        actions = torch.stack(actions)
        next_states = torch.stack(next_states)
        targets = torch.stack(rewards).squeeze()
        dones_mask = 1 - torch.stack(dones).type(torch.int)
        targets += (
            self.gamma * dones_mask * torch.amax(self.target_model(next_states), dim=1)
        )
        preds = self.model(states)

        loss = self.criterion(preds, targets)
        self.optimizer.zero_grad()
        loss.backward()
        self.optimizer.step()
        loss.backward()
        self.optimizer.step()

    def update_target_model(self):
        self.target_model.load_state_dict(self.model.state_dict())
        self.target_model.eval()

    def configure_logger(self, **kwargs):
        wandb.Settings
        wandb.init(**kwargs)

    def configure_optimizer(self, fn, **kwargs):
        if fn == "adam":
            self.optimizer = torch.optim.Adam(self.model.parameters(), **kwargs)
        elif fn == "adamw":
            self.optimizer = torch.optim.AdamW(self.model.parameters(), **kwargs)
        elif fn == "sgd":
            self.optimizer = torch.optim.SGD(self.model.parameters(), **kwargs)
        else:
            raise ValueError(f"Optimizer {fn} not supported")

    def learn(
        self,
        num_episodes: int,
        max_steps: int,
        render: bool = False,
        log_freq: Optional[int] = None,
        save_freq: int = 100,
        save_dir: str = "models",
        load_path: Optional[str] = None,
        gamma: float = 0.99,
        epsilon: float = 1.0,
        epsilon_decay: float = 0.995,
        epsilon_min: float = 0.1,
        tau: float = 0.1,
        batch_size: int = 64,
        memory_size: int = 10000,
    ) -> None:
        self.gamma = gamma
        self.epsilon_decay = epsilon_decay
        self.epsilon_min = epsilon_min
        self.tau = tau
        self.batch_size = batch_size
        self.optimizer

        self.memory = deque(maxlen=memory_size)

        if save_dir is not None:
            save_dir = Path(save_dir)
            save_dir.mkdir(parents=True, exist_ok=True)

        if load_path is not None:
            checkpoint = torch.load(load_path)
            self.model.load_state_dict(checkpoint["model"])
            self.optimizer.load_state_dict(checkpoint["optimizer"])
            self.epsilon = checkpoint["epsilon"]
            start_episode = checkpoint["episode"]
        else:
            self.epsilon = epsilon
            start_episode = 0

        self.model.train()
        for e in range(start_episode, num_episodes):
            print(f"Episode {e}/{num_episodes}")
            state, _ = self.env.reset()
            total_reward = 0
            running = True
            step = 0
            while running:
                action = self.choose_action(state, epsilon)
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
