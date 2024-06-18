import torch
from torch import nn


class DQN(nn.Module):
    def __init__(self, width, height, numSquares):
        super(DQN, self).__init__()
        self.fc1 = nn.Linear(width * height, 128)
        self.fc2 = nn.Linear(128, 128)
        self.fc3 = nn.Linear(128, numSquares)

    def choose_action(self, state, epsilon):
        if np.random.rand() < epsilon:
            return np.random.randint(0, numSquares)
        else:
            state = torch.tensor(state, dtype=torch.float32)
            return torch.argmax(self(state)).item()

    def forward(self, x):
        x = torch.relu(self.fc1(x))
        x = torch.relu(self.fc2(x))
        x = self.fc3(x)
        return x
