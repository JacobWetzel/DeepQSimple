import gymnasium as gym
import numpy as np
import torch
from gymnasium import spaces


class MouseEnv(gym.Env):
    def __init__(self, height, width, num_destinations, env_repr="grid"):
        super().__init__()
        self.height = height
        self.width = width
        self.num_destinations = num_destinations
        self.remaining_destinations = num_destinations
        self.env_repr = env_repr

        self._action_to_direction = {
            0: torch.tensor([0, -1]),
            1: torch.tensor([0, 1]),
            2: torch.tensor([-1, 0]),
            3: torch.tensor([1, 0]),
        }

        if self.env_repr == "grid":
            self.observation_space = spaces.MultiBinary((2, height, width))
        elif self.env_repr == "pos":
            low = np.repeat(
                np.zeros((1, 2), dtype=np.int32), num_destinations + 1, axis=0
            )
            high = (
                np.repeat(
                    np.array([[height - 1, width - 1]], dtype=np.int32),
                    num_destinations + 1,
                    axis=0,
                ),
            )
            self.observation_space = spaces.Box(
                low=low,
                high=high,
                dtype=np.int32,
            )
        else:
            raise ValueError(f"Invalid env_repr: {self.env_repr}")

    @staticmethod
    def _idx_to_pos(idx, width):
        return idx // width, idx % width

    def _get_obs(self):
        return self.state.clone()

    def _get_info(self):
        return {
            "cur_pos": self.cur_pos.clone(),
            "destination_set": self.destination_set,
        }

    def reset(self, seed=None, options=None):
        super().reset(seed=seed)

        state = torch.tensor(
            np.random.choice(
                self.height * self.width, self.num_destinations + 1, replace=False
            )
        )
        state = torch.stack(self._idx_to_pos(state, self.width), dim=1)

        cur_pos = state[0]
        destinations = state[1:]

        if self.env_repr == "grid":
            state = torch.zeros(2, self.height, self.width)
            state[0, cur_pos[0], cur_pos[1]] = 1
            state[1, destinations[:, 0], destinations[:, 1]] = 1

        self.cur_pos = cur_pos
        self.destination_set = set(map(tuple, destinations.tolist()))
        self.state = state

        return self._get_obs(), self._get_info()

    def _clamp_pos(self, pos):
        return torch.clamp(pos, 0, torch.tensor([self.height - 1, self.width - 1]))

    def step(self, action):
        # Move the mouse to the next square
        next_pos = self.cur_pos + self._action_to_direction[action]
        next_pos = self._clamp_pos(next_pos)
        terminated = False
        truncated = False
        reward = 0
        if self.env_repr == "grid":
            self.state[*self.cur_pos] = 0
            if self.state[1, *next_pos] == 1:
                if self.remaining_destinations == 1:
                    terminated = True
                self.state[1, *next_pos] = 0
                reward += 1
                self.remaining_destinations -= 1
            self.state[0, *next_pos] = 1
        elif self.env_repr == "pos":
            self.state[0] = next_pos

        return self._get_obs(), reward, terminated, truncated, self._get_info()
