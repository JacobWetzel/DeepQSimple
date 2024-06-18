import random
from collections import deque
from math import prod

import numpy as np
import torch
from arrow import get
from torch import nn

from utils import AttrDict, get_criterion, get_optimizer


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


class MouseEnv:
    def __init__(self, height, width, num_destinations, env_repr="grid"):
        self.height = height
        self.width = width
        self.num_destinations = num_destinations
        self.remaining_destinations = num_destinations
        self.env_repr = env_repr

        self.direction_dict = {
            0: torch.tensor([0, -1]),
            1: torch.tensor([0, 1]),
            2: torch.tensor([-1, 0]),
            3: torch.tensor([1, 0]),
        }
        self.state, self.cur_pos = self.new_state()
        if self.env_repr == "grid":
            self.obs_shape = self.state.shape
        elif self.env_repr == "pos":
            self.obs_shape = prod(self.state.shape)
        else:
            raise ValueError(f"Invalid env_repr: {self.env_repr}")

    @staticmethod
    def idx_to_pos(idx, width):
        return idx // width, idx % width

    def observe(self):
        return self.state

    def new_state(self, height, width, num_destinations):
        # Randomly choose one start square and num_destinations squares
        state = torch.tensor(
            np.random.choice(height * width, num_destinations + 1, replace=False)
        )
        state = torch.stack(self.idx_to_pos(state, width), dim=1)
        cur_pos = state[0]

        destinations = state[1:]
        if self.env_repr == "grid":
            state = torch.zeros(height, width)
            state[cur_pos[0], cur_pos[1]] = 2
            state[destinations[:, 0], destinations[:, 1]] = 1

        self.destinations_set = set(map(tuple, destinations.tolist()))

        return state, cur_pos

    def reward(self, state, action):
        next_pos = self.cur_pos + self.direction_dict[action]
        if self.env_repr == "grid":
            if state[next_pos[0], next_pos[1]] == 1:
                return 1
            else:
                return 0
        elif self.env_repr == "pos":
            if (next_pos[0], next_pos[1]) in self.destinations_set:
                return 1
            else:
                return 0

    def step(self, action):
        # Move the mouse to the next square
        prev_pos = self.cur_pos
        self.cur_pos = self.cur_pos + self.direction_dict[action]
        done = False
        if self.env_repr == "grid":
            self.state[prev_pos[0], prev_pos[1]] = 0
            if (
                self.state[self.cur_pos[0], self.cur_pos[1]] == 1
                and self.remaining_destinations == 1
            ):
                done = True
            self.remaining_destinations -= 1
            self.state[self.cur_pos[0], self.cur_pos[1]] = 2
            self.state[self.cur_pos[0], self.cur_pos[1]] = 2
        elif self.env_repr == "pos":
            self.state[0] = self.cur_pos

        return self.state, done


class DQN:
    def __init__(
        self,
        env,
        memory_size,
        optimizer,
        criterion,
        optimizer_kwargs={"lr": 0.001},
        criterion_kwargs=None,
        gamma=0.99,
        epsilon=1.0,
        epsilon_decay=0.995,
        epsilon_min=0.1,
        tau=0.1,
        batch_size=32,
    ):
        self.criterion = criterion
        self.gamma = gamma
        self.epsilon = epsilon
        self.epsilon_decay = epsilon_decay
        self.epsilon_min = epsilon_min
        self.tau = tau
        self.batch_size = batch_size

        self.model = MLP(env.obs_shape, env.num_actions)
        self.target_model = MLP(env.obs_shape, env.num_actions)
        self.target_model.load_state_dict(self.model.state_dict())
        self.target_model.eval()

        if optimizer_kwargs is None:
            optimizer_kwargs = {}
        if criterion_kwargs is None:
            criterion_kwargs = {}
        self.optimizer = get_optimizer(optimizer, self.model, optimizer_kwargs)
        self.criterion = get_criterion(criterion, criterion_kwargs)

        self.memory = deque(maxlen=memory_size)

    def choose_action(self, state, epsilon):
        if torch.randn() < epsilon:
            return torch.randint(0, self.num_actions, state.shape[0])
        else:
            q_values = self.model(state)
            return torch.argmax(q_values[0], dim=1)

    def sample(self, epsilon):
        action = self.choose_action(state, epsilon)

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
