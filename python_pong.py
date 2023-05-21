import pygame
import random

# Initialize Pygame
pygame.init()

# Set up the display
screen_width = 640
screen_height = 480
game_window = pygame.display.set_mode((screen_width, screen_height))
pygame.display.set_caption("Pong Game")

# Set up colors
white = (255, 255, 255)
black = (0, 0, 0)

# Set up ball
ball_x = int(screen_width / 2)
ball_y = int(screen_height / 2)
ball_speed_x = 3 * random.choice((1, -1))
ball_speed_y = 3 * random.choice((1, -1))
ball_radius = 10

# Set up paddles
paddle_width = 15
paddle_height = 60
paddle_speed = 5

player_paddle_x = 25
player_paddle_y = int(screen_height / 2) - int(paddle_height / 2)

ai_paddle_x = screen_width - paddle_width - 25
ai_paddle_y = int(screen_height / 2) - int(paddle_height / 2)

# Set up game clock
clock = pygame.time.Clock()

# Set up score
player_score = 0
ai_score = 0
font = pygame.font.Font(None, 36)

# Pre-render score text
player_text = font.render(str(player_score), True, white)
ai_text = font.render(str(ai_score), True, white)

# Set up game over message
game_over_font = pygame.font.Font(None, 48)
game_over_text = game_over_font.render("GAME OVER", True, white)
game_over_text_rect = game_over_text.get_rect(center=(screen_width/2, screen_height/2))

# Set up game start message
game_start_font = pygame.font.Font(None, 48)
game_start_text = game_start_font.render("PRESS SPACE TO START", True, white)
game_start_text_rect = game_start_text.get_rect(center=(screen_width/2, screen_height/2))

# Set up game state
game_running = False

# Create a secondary surface for double buffering
buffer_surface = pygame.Surface((screen_width, screen_height))

# Game loop
running = True
while running:
    # Handle events
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_SPACE:
                if not game_running:
                    game_running = True
                    player_score = 0
                    ai_score = 0
                    ball_x = int(screen_width / 2)
                    ball_y = int(screen_height / 2)
                    ball_speed_x = 3 * random.choice((1, -1))
                    ball_speed_y = 3 * random.choice((1, -1))

    if game_running:
        # Move player paddle
        keys = pygame.key.get_pressed()
        if keys[pygame.K_UP]:
            player_paddle_y -= paddle_speed
        if keys[pygame.K_DOWN]:
            player_paddle_y += paddle_speed

        # Move AI paddle
        ai_paddle_y += ball_speed_y
        if ai_paddle_y < 0 or ai_paddle_y + paddle_height > screen_height:
            ai_paddle_y -= ball_speed_y

        # Move ball
        ball_x += ball_speed_x
        ball_y += ball_speed_y

        # Check for ball collision with walls
        if ball_y - ball_radius < 0 or ball_y + ball_radius > screen_height:
            ball_speed_y = -ball_speed_y

        # Check for ball collision with player paddle
        if ball_x - ball_radius < player_paddle_x + paddle_width and \
           ball_y > player_paddle_y and \
           ball_y < player_paddle_y + paddle_height:
            ball_speed_x = -ball_speed_x
            ball_speed_y += random.uniform(-1, 1)

            # Increase player score
            player_score += 1
            player_text = font.render(str(player_score), True, white)

        # Check for ball collision with AI paddle
        if ball_x + ball_radius > ai_paddle_x and \
           ball_y > ai_paddle_y and \
           ball_y < ai_paddle_y + paddle_height:
            ball_speed_x = -ball_speed_x
            ball_speed_y += random.uniform(-1, 1)

            # Increase AI score
            ai_score += 1
            ai_text = font.render(str(ai_score), True, white)

        # Check for game over
        if player_score >= 10 or ai_score >= 10:
            game_running = False

    # Draw graphics on buffer surface
    buffer_surface.fill(black)  # Clear buffer surface

    if game_running:
        # Draw ball on buffer surface
        pygame.draw.circle(buffer_surface, white, (ball_x, ball_y), ball_radius)

        # Draw player paddle on buffer surface
        pygame.draw.rect(buffer_surface, white, (player_paddle_x, player_paddle_y, paddle_width, paddle_height))

        # Draw AI paddle on buffer surface
        pygame.draw.rect(buffer_surface, white, (ai_paddle_x, ai_paddle_y, paddle_width, paddle_height))

        # Draw score on buffer surface
        buffer_surface.blit(player_text, (screen_width / 4, 10))
        buffer_surface.blit(ai_text, (screen_width * 3 / 4, 10))

    else:
        # Draw game over or game start message on buffer surface
        if player_score >= 10:
            buffer_surface.blit(game_over_text, game_over_text_rect)
        else:
            buffer_surface.blit(game_start_text, game_start_text_rect)

    # Blit buffer surface to game window
    game_window.blit(buffer_surface, (0, 0))

    # Update the display
    pygame.display.update()

    # Set game clock tick rate
    clock.tick(60)

# Quit the game
pygame.quit()
