import pygame

# Initialize Pygame
pygame.init()

# Set up the window
screen_width = 640
screen_height = 640
screen = pygame.display.set_mode((screen_width, screen_height))
pygame.display.set_caption("Chessboard")

# Define the chessboard dimensions
num_rows = 8
num_cols = 8
cell_size = 80

# Define the chessboard data
chessboard = [[0 for j in range(num_cols)] for i in range(num_rows)]

# Initialize the chessboard data
for i in range(num_rows):
    for j in range(num_cols):
        if (i + j) % 2 == 0:
            chessboard[i][j] = 1

# Draw the chessboard
while True:
    # Handle events
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            exit()

    # Clear the screen
    screen.fill((255, 255, 255))

    # Draw the chessboard cells
    for i in range(num_rows):
        for j in range(num_cols):
            if chessboard[i][j] == 1:
                color = (0, 0, 0)
            else:
                color = (255, 255, 255)
            rect = pygame.Rect(j * cell_size, i * cell_size, cell_size, cell_size)
            pygame.draw.rect(screen, color, rect)

    # Update the screen
    pygame.display.update()
