import gym


def moveUp(state):
    #incr y val
    return

def moveDown(state):
    #decr y val
    return

def moveLeft(state):
    #decr x val
    return

def moveRight(state):
    #incr x val
    return

def up(state):
    return

def down(state):
    return

def left(state):
    return

def right(state):
    return


actions = [goUp, goDown, goLeft, goRight]



game_action_space = gym.spaces.Discrete(len(actions))

observations = ['xVal', 'yVal', 'distanceFromGoal']
