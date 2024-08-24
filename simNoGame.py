import tensorflow as tf
import numpy as np
from collections import deque
import random

# Check TensorFlow version and GPU availability
print("TensorFlow version:", tf.__version__)
print("Num GPUs Available: ", len(tf.config.list_physical_devices('GPU')))

# Example data (replace with your own data)
features = np.random.rand(10000, 10)
labels = np.random.randint(2, size=(10000, 1))

# Hyperparameters
batch_size = 20480  # Increase batch size to improve GPU utilization
memory_size = 10000
num_episodes = 1000
alpha = 0.001  # Learning rate
gamma = 0.99  # Discount factor
epsilon = 1.0  # Exploration rate
epsilon_min = 0.1
epsilon_decay = 0.995
update_target_freq = 10  # Frequency to update the target network

# Data pipeline
dataset = tf.data.Dataset.from_tensor_slices((features, labels))
dataset = dataset.shuffle(buffer_size=1024).batch(batch_size).prefetch(tf.data.AUTOTUNE)

# Define the neural network
model = tf.keras.Sequential([
    tf.keras.layers.Input(shape=(10,)),
    tf.keras.layers.Dense(24, activation='relu'),
    tf.keras.layers.Dense(24, activation='relu'),
    tf.keras.layers.Dense(1, activation='sigmoid')  # For binary classification
])
model.compile(optimizer=tf.keras.optimizers.Adam(learning_rate=alpha), loss='binary_crossentropy', metrics=['accuracy'])

# Train the model on the dataset
model.fit(dataset, epochs=10)

# Initialize the target network
target_model = tf.keras.models.clone_model(model)
target_model.set_weights(model.get_weights())

# Define your environment class
class MySquare:
    def __init__(self, seed):
        self.xPos = 750
        self.yPos = 875
        self.width = 10
        self.height = 10
        self.active = False  # Initially, no square is active
        self.rng = random.Random(seed)
        self.color = (50, 200, 200)

    def moveInDir(self, direction):
        if self.active:
            self.xPos += direction[0] * 10
            self.yPos += direction[1] * 10

    def setRColor(self, colors):
        self.color = colors

    def getRColor(self):
        return self.color

    def setPos(self, x, y):
        self.xPos = x
        self.yPos = y

    def getRect(self):
        return (self.xPos, self.yPos, self.width, self.height)

    def activate(self):
        self.active = True

    def deactivate(self):
        self.active = False

def isCol(sq):
    if (((sq.xPos >= 1500 / 6 and sq.xPos <= 1510 / 6) or (sq.xPos + 10 >= 1500 / 6 and sq.xPos + 10 <= 1510 / 6)) and
            ((sq.yPos >= 100 and sq.yPos <= 110) or (sq.yPos + 10 >= 100 and sq.yPos + 10 <= 110))):
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
            target += gamma * np.amax(target_model.predict(next_state.reshape(1, -1), verbose=0)[0])
        target_f = model.predict(state.reshape(1, -1), verbose=0)
        target_f[0][action] = target
        model.fit(state.reshape(1, -1), target_f, epochs=1, verbose=0)

def update_target_model():
    target_model.set_weights(model.get_weights())

# Initialize squares
numSquares = 10
squares = [MySquare(i + 1) for i in range(numSquares)]
squares[0].activate()

current_square_index = 0
destination = MySquare(100)
destination.setRColor((200, 100, 100))
destination.setPos(1500 / 6, 100)

# Initialize experience replay memory
memory = deque(maxlen=memory_size)

# Reinforcement learning training loop
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
