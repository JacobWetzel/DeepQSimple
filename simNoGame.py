import random
from collections import deque

import numpy as np
import torch

# Hyperparameters
alpha = 0.001  # Learning rate
gamma = 0.99  # Discount factor
epsilon = 1.0  # Exploration rate
epsilon_min = 0.1
epsilon_decay = 0.995
batch_size = 64
memory_size = 10000
num_episodes = 1000
update_target_freq = 10  # Frequency to update the target network

# Environment parameters
width, height = 1500, 1000
numSquares = 10
destination_pos = (1500 // 6, 100)

# Define the neural network

# Experience replay memory
memory = deque(maxlen=memory_size)


class MySquare:
    def __init__(self, seed):
        self.xPos = width // 2
        self.yPos = (height * 7) // 8
        self.width = 10
        self.height = 10
        self.active = False  # Initially, no square is active
        self.rng = random.Random(seed)

    def moveInDir(self, direction):
        if self.active:
            self.xPos += direction[0] * 10
            self.yPos += direction[1] * 10

    def setPos(self, x, y):
        self.xPos = x
        self.yPos = y

    def activate(self):
        self.active = True

    def deactivate(self):
        self.active = False


def isCol(sq):
    if (
        (sq.xPos >= destination_pos[0] and sq.xPos <= destination_pos[0] + 10)
        or (
            sq.xPos + 10 >= destination_pos[0]
            and sq.xPos + 10 <= destination_pos[0] + 10
        )
    ) and (
        (sq.yPos >= destination_pos[1] and sq.yPos <= destination_pos[1] + 10)
        or (
            sq.yPos + 10 >= destination_pos[1]
            and sq.yPos + 10 <= destination_pos[1] + 10
        )
    ):
        return True
    return False


def get_state(squares, current_square_index):
    active_square = squares[current_square_index]
    return np.array([active_square.xPos, active_square.yPos, current_square_index])


def choose_action(state, epsilon):
    if np.random.rand() < epsilon:
        return random.randint(0, 4)
    else:
        q_values = model.predict(state.reshape(1, -1), verbose=0)
        return np.argmax(q_values[0])


def remember(state, action, reward, next_state, done):
    memory.append((state, action, reward, next_state, done))


def replay():
    if len(memory) < batch_size:
        return

    minibatch = random.sample(memory, batch_size)
    for state, action, reward, next_state, done in minibatch:
        target = reward
        if not done:
            target += gamma * np.amax(
                target_model.predict(next_state.reshape(1, -1), verbose=0)[0]
            )
        target_f = model.predict(state.reshape(1, -1), verbose=0)
        target_f[0][action] = target
        model.fit(state.reshape(1, -1), target_f, epochs=1, verbose=0)


def update_target_model():
    target_model.set_weights(model.get_weights())


# Initialize squares
def train():
    squares = [MySquare(i + 1) for i in range(numSquares)]
    squares[0].activate()

    current_square_index = 0

    for e in range(num_episodes):
        state = get_state(squares, current_square_index)
        total_reward = 0
        running = True
        step = 0
        while running:
            action = choose_action(state, epsilon)
            if action == 0:
                direction = [0, -1]
            elif action == 1:
                direction = [0, 1]
            elif action == 2:
                direction = [-1, 0]
            elif action == 3:
                direction = [1, 0]
            elif action == 4:
                squares[current_square_index].deactivate()
                current_square_index = (current_square_index + 1) % numSquares
                squares[current_square_index].activate()
                direction = [0, 0]

            if squares[current_square_index].active:
                squares[current_square_index].moveInDir(direction)

            if (
                squares[current_square_index].xPos >= 1500
                or squares[current_square_index].xPos <= 0
                or squares[current_square_index].yPos >= 1000
                or squares[current_square_index].yPos <= 0
            ):
                squares[current_square_index].xPos = 1500 // 2
                squares[current_square_index].yPos = (1000 * 7) // 8

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
