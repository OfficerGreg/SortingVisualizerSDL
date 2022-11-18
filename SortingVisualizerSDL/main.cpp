#include <SDL.h>

#include <algorithm>
#include <iostream>
#include <random>
#include <ranges>

#undef main 

const unsigned int screenWitdh = 1200;
const unsigned int screenHeight = 720;

unsigned int v_size = { };

void bubbleSort() {

}

void cocktailSort() {

}

//red = i //blue = j
void draw_state(std::vector<int>& v, SDL_Renderer* renderer, unsigned int red, unsigned int blue) {
	unsigned int index = 0;
	for (int i : v) {
		if(index == red)
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		else if(index == blue)
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		else
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawLine(renderer, index, v_size, index, i);
		index++;
	}

}

int main() {

	std::vector<int> v;
	v_size = 720;

	for (int i = 0; i < v_size; i++)
		v.push_back(i);


	auto rng = std::default_random_engine{};
	std::shuffle(std::begin(v), std::end(v), rng);

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_CreateWindowAndRenderer(1200, 720, 0, &window, &renderer);
	SDL_RenderSetScale(renderer, 1, 1);

	bool swapped = true;
	int start = 0;
	int end = v.size() - 1;

	while (swapped) {
		swapped = false;
		for (int i = start; i < end; ++i) {
			if (v[i] > v[i + 1]) {
				std::swap(v[i], v[i + 1]);
				swapped = true;
			}
		}
		if (!swapped)
			break;
		swapped = false;
		--end;
		for (int i = end - 1; i >= start; --i) {
			if (v[i] > v[i + 1]) {
				std::swap(v[i], v[i + 1]);
				swapped = true;
			}
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);

			draw_state(v, renderer, i, i);
			//show window
			SDL_RenderPresent(renderer);
		}
		++start;
	}
	
	SDL_Event e; bool quit = false; while (quit == false) { while (SDL_PollEvent(&e)) { if (e.type == SDL_QUIT) quit = true; } }

	for (int i : v)
		std::cout << i << " ";
	if (std::ranges::is_sorted(v))
		std::cout << "sorted\n";
}