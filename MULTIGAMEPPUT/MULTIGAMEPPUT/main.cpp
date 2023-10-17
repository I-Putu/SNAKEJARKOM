#include <SDL.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int GRID_SIZE = 20; // Size of each grid cell
const int SNAKE_SPEED = 10; // Adjust speed as needed

SDL_Renderer* renderer;
int snakeX[100], snakeY[100]; // Arrays to store snake positions
int snakeLength = 1; // Initial length of the snake
int direction = 0; // 0: right, 1: left, 2: up, 3: down
int foodX, foodY; // Food position
int gameover = 0;

void generateFood() {
	foodX = rand() % (SCREEN_WIDTH / GRID_SIZE) * GRID_SIZE;
	foodY = rand() % (SCREEN_HEIGHT / GRID_SIZE) * GRID_SIZE;
}

void moveSnake() {
	// Move the snake by adding a new head in the direction of motion
	for (int i = snakeLength - 1; i > 0; i--) {
		snakeX[i] = snakeX[i - 1];
		snakeY[i] = snakeY[i - 1];
	}
	switch (direction) {
	case 0: snakeX[0] += GRID_SIZE; break; // Right
	case 1: snakeX[0] -= GRID_SIZE; break; // Left
	case 2: snakeY[0] -= GRID_SIZE; break; // Up
	case 3: snakeY[0] += GRID_SIZE; break; // Down
	}

	// Check for collisions with food
	if (snakeX[0] == foodX && snakeY[0] == foodY) {
		snakeLength++;
		generateFood();
	}

	// Check for collisions with the wall or itself
	if (snakeX[0] < 0 || snakeX[0] >= SCREEN_WIDTH || snakeY[0] < 0 || snakeY[0] >= SCREEN_HEIGHT) {
		gameover = 1;
	}
	for (int i = 1; i < snakeLength; i++) {
		if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
			gameover = 1;
		}
	}
}

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	generateFood(); // Generate initial food

	srand(time(0)); // Seed random number generator
	snakeX[0] = SCREEN_WIDTH / 2;
	snakeY[0] = SCREEN_HEIGHT / 2;

	SDL_Event event;
	while (!gameover) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				gameover = 1;
			}
			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
				case SDLK_RIGHT:
					if (direction != 1) direction = 0; break; // Right
				case SDLK_LEFT:
					if (direction != 0) direction = 1; break; // Left
				case SDLK_UP:
					if (direction != 3) direction = 2; break; // Up
				case SDLK_DOWN:
					if (direction != 2) direction = 3; break; // Down
				}
			}
		}

		moveSnake();

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		// Draw the snake
		for (int i = 0; i < snakeLength; i++) {
			SDL_Rect rect = { snakeX[i], snakeY[i], GRID_SIZE, GRID_SIZE };
			SDL_RenderFillRect(renderer, &rect);
		}

		// Draw the food
		SDL_Rect foodRect = { foodX, foodY, GRID_SIZE, GRID_SIZE };
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(renderer, &foodRect);

		SDL_RenderPresent(renderer);
		SDL_Delay(1000 / SNAKE_SPEED); // Control game speed

	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
