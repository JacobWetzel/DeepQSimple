import pygame
import sys

# Initialize Pygame
pygame.init()

# Set up the screen
screen_width = 1600
screen_height = 1000
screen = pygame.display.set_mode((screen_width, screen_height))
pygame.display.set_caption("Move the Square")

# Define square properties
square_size = 10
square_x = (screen_width - square_size) // 2
square_y = (screen_height - square_size) // 2



square_color = (255, 0, 0)  # Red color
background_color = (0, 0, 0)  # Black background

#class blocks(xOrigin = 0, yOrigin = 0, x = 0, y = 0):
#    blockColor = (100, 0, 255)
    



# Movement speed
max_walk = 250.0 / 16
walk_accel = 1.8

accelBand0 = 250.0 / 64.0
accelBand1 = 250.0 / 256.0
accelBand2 = 250.0 / 512.0

deccelBand0 = 250.0 / 64.0
deccelBand1 = 250.0 / 256.0
deccelBand2 = 250.0 / 512.0


moveX = 0
moveY = 0
moveZ = 0
isJump = False
isOnGround = True


def groundDeccelerate(move_speed):
    if(abs(move_speed) < 50):
        mw = deccelBand0
    elif(abs(move_speed) < 150):
        mw = deccelBand1
    else:
        mw = deccelBand2
    if(move_speed > 0):
        return max(move_speed - mw, 0)
    elif(move_speed < 0):
        return min(move_speed + mw, 0)
    elif(move_speed == 0):
        return 0

def KeyAccelerate(move_speed, key):
    if(abs(move_speed) < 50):
        mw = accelBand0
    elif(abs(move_speed) < 150):
        mw = accelBand1
    else:
        mw = accelBand2
    if(key == 0):
        return max(move_speed - mw, -max_walk)
    elif(key == 1):
        return min(move_speed + mw, max_walk)
    elif(key == 2):
        return max(move_speed - mw, -max_walk)
    elif(key == 3):
        return min(move_speed + mw, max_walk)
    return move_speed


def jumpAccelerate(move_speed, jumpFrames):
    
    return move_speed


# Main game loop
running = True
while running:
    # Handle events
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False


    # Get keys pressed
    keys = pygame.key.get_pressed()
    if isOnGround:
        if keys[pygame.K_w]:
            moveY = KeyAccelerate(moveY, 0)                
        elif keys[pygame.K_s]:
            moveY = KeyAccelerate(moveY, 1)
        else:
            moveY = groundDeccelerate(moveY)
     
        if keys[pygame.K_a]:
            moveX = KeyAccelerate(moveX, 2)
        elif keys[pygame.K_d]:
            moveX = KeyAccelerate(moveX, 3)
        else:
            moveX = groundDeccelerate(moveX)
            
    square_x += moveX
    square_y += moveY
    
    if(moveX > 0):
        square_color = (0, 255, 0)
    elif(moveX < 0):
        square_color = (255, 0, 0)
    else:
        square_color = (0, 0, 255)

    # Keep the square within the screen bounds
    if square_x < 0:
        square_x = 0
    if square_x > screen_width - square_size:
        square_x = screen_width - square_size
    if square_y < 0:
        square_y = 0
    if square_y > screen_height - square_size:
        square_y = screen_height - square_size

    # Draw everything
    screen.fill(background_color)
    pygame.draw.rect(screen, square_color, (square_x, square_y, square_size, square_size))
    pygame.display.flip()

    # Control the frame rate
    pygame.time.Clock().tick(60)

# Clean up and quit
pygame.quit()
sys.exit()
