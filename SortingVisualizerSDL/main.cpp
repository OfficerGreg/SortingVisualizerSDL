#include <SDL.h>

#include <algorithm>
#include <iostream>
#include <random>
#include <ranges>


#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800


SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

unsigned int v_size = 400;
std::vector<int> v;

unsigned int rectangle_width = SCREEN_WIDTH / v_size;

bool is_finished = false;

void randVector() {
	for (int i = 0; i < v_size; i++)
		v.push_back(i);

	auto rng = std::default_random_engine{};
	std::shuffle(std::begin(v), std::end(v), rng);
}


bool init() {
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "Couldn't initialize SDL. SDL_Error: " << SDL_GetError();
		success = false;
	}
	else {
		if (!(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))) {
			std::cout << "Warning: Linear Texture Filtering not enabled.\n";
		}

		window = SDL_CreateWindow("Sorting Visualizer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
		if (window == NULL) {
			std::cout << "Couldn't create window. SDL_Error: " << SDL_GetError();
			success = false;
		}
		else {
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if (renderer == NULL) {
				std::cout << "Couldn't create renderer. SDL_Error: " << SDL_GetError();
				success = false;
			}
		}
	}
	return success;
}
void close()
{
	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	SDL_DestroyWindow(window);
	window = NULL;

	SDL_Quit();
}

void draw(unsigned int y, unsigned int z) {
	int x = { 0 };
	int index = {};
	for (int i = 0; i < v.size();  i++) {
		SDL_Rect r = {index, SCREEN_HEIGHT - v[i], rectangle_width, v[i]};
		if (x == y) 
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		else if (x == z) 
			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		else
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(renderer, &r);
		index+=rectangle_width;
		x++;
	}
}
void bubbleSort() {

	for (int i = 0; i < v.size(); i++) {
		for (int j = 0; j < v.size() - 1; j++) {
			if (v[j] > v[j + 1]) {
				std::swap(v[j], v[j + 1]);
			}
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);
			draw(i, j+1);
			SDL_RenderPresent(renderer);
		}
	}
	std::cout << "Sorted\n";
}


int main(int argc, char* args[]) {
	randVector();

	if (!init())
		std::cout << "Failed to initialize!\n";
	else
	{
		bool quit = false;

		SDL_Event e;

		while (!quit) {
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT)
					quit = true;
				else if (e.type == SDL_KEYDOWN) {
					switch (e.key.keysym.sym) {
					case SDLK_ESCAPE:
						quit = true;
						break;
					
					case SDLK_1:
						is_finished = false;
						bubbleSort();
						is_finished = true;
						break;
					}
				}
			}

		}
	}
	close();
		
	return 0;
}