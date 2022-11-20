#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_audio.h>

#include <algorithm>
#include <iostream>
#include <random>
#include <ranges>


#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 720

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

unsigned int speed = 0;

unsigned int v_size = 200;
std::vector<int> v;

unsigned int rectangle_width = SCREEN_WIDTH / v_size;

bool is_finished = false;

bool init() {

	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
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
			if (TTF_Init() == -1)
			{
				std::cout << ("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
				success = false;
			}
		}
	}
	return success;
}



void randVector() {
	v.clear();

	for (int i = 0; i < v_size; i++)
		v.push_back(i);

	auto rng = std::default_random_engine{};
	std::shuffle(std::begin(v), std::end(v), rng);
}

void close()
{
	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	SDL_DestroyWindow(window);
	window = NULL;

	SDL_Quit();
	TTF_Quit();
}

void draw(unsigned int y, unsigned int z, unsigned int f) {
	int x = { 0 };
	int index = { 0 };
	for (int i = 0; i < v.size(); i++) {
		SDL_Rect r = { index, SCREEN_HEIGHT - v[i], rectangle_width, v[i] };
		if (x == y)
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		else if (x == z)
			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		else if (x == f)
			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		else if (std::ranges::is_sorted(v)){
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		}
		else
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(renderer, &r);
		index += rectangle_width;
		x++;
	}

}

//Sorting algorithm
void bubbleSort() {

	for (int i = 0; i < v.size(); i++) {
		for (int j = 0; j < v.size() - 1; j++) {
			if (v[j] > v[j + 1]) {
				std::swap(v[j], v[j + 1]);
			}
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);
			draw(i, j+1, 0);
			SDL_RenderPresent(renderer);
			SDL_Delay(speed);
		}
	}

}

void selectionSort() {
	int i, j, min_idx = {};
	for (i = 0; i < v.size() - 1; i++) {
		min_idx = i;
		for (j = i + 1; j < v.size(); j++){
			if (v[j] < v[min_idx])
				min_idx = j;
		}
			if (min_idx != i)
				std::swap(v[min_idx], v[i]);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);
			draw(i, j + 1, 0);
			SDL_RenderPresent(renderer);
			SDL_Delay(speed);
	}
	std::cout << "Sorted\n";
}

void insertionSort() {
	for (int i = 1; i < v.size(); i++)
	{
		int key = v[i];
		int j = i - 1;
		while (j >= 0 && v[j] > key)
		{
			v[j + 1] = v[j];
			j = j - 1;
		}
		v[j + 1] = key;
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		draw(i, j, 0);
		SDL_RenderPresent(renderer);
		SDL_Delay(speed);
	}
	std::cout << "Sorted\n";
}

//void quickSort(std::vector<int> v,int low, int high) {
//	if (low < high) {
		//int pi = partition(v, low, high);

		// Separately sort elements before
		// partition and after partition
//		quickSort(v, low, pi - 1);
//		quickSort(v, pi + 1, high);
//		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//		SDL_RenderClear(renderer);
//		draw(low, high, 0);
//		SDL_RenderPresent(renderer);
//		SDL_Delay(speed);
//	}
//	std::cout << "Sorted\n";
//}

void mergeSortedIntervals(std::vector<int>& v, int s, int m, int e) {

	// temp is used to temporary store the vector obtained by merging
	// elements from [s to m] and [m+1 to e] in v
	std::vector<int> temp;

	int i, j;
	i = s;
	j = m + 1;
	while (i <= m && j <= e) {

		if (v[i] <= v[j]) {
			temp.push_back(v[i]);
			++i;
		}
		else {
			temp.push_back(v[j]);
			++j;
		}

	}
	while (i <= m) {
		temp.push_back(v[i]);
		++i;
	}

	while (j <= e) {
		temp.push_back(v[j]);
		++j;
	}

	for (int i = s; i <= e; ++i)
		v[i] = temp[i - s];
}

void mergeSort(std::vector<int>& v, int s, int e) {
	if (s < e) {
		int m = (s + e) / 2;
		mergeSort(v, s, m);
		mergeSort(v, m + 1, e);
		mergeSortedIntervals(v, s, m, e);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		draw(s, m, e);
		SDL_RenderPresent(renderer);
		SDL_Delay(speed);
	}
	std::cout << "Sorted\n";
}



int main(int argc, char* args[]) {

	SDL_AudioDeviceID audio_device;

	// AUDIO
	SDL_AudioSpec audio_spec;
	SDL_zero(audio_spec);
	audio_spec.freq = 44100;
	audio_spec.format = AUDIO_S16SYS;
	audio_spec.channels = 1;
	audio_spec.samples = 1024;
	audio_spec.callback = NULL;

	audio_device = SDL_OpenAudioDevice(
		NULL, 0, &audio_spec, NULL, 0);

	//TEXT
	if (!init())
		std::cout << "Failed to initialize!\n";
	else {
		TTF_Init();
		TTF_Font* font = TTF_OpenFont("./font/8bitOperatorPlusSC-Regular.ttf", 48);

		if (font == nullptr) {
			printf("TTF_Init: %s\n", TTF_GetError());
			exit(1);
		}
	const char* controlsT = "CONTROLS:";
	const char* exitT = "- press { ESC } to exit program";
	const char* randomVT = "- press { R } to randomize vector *not visual*";
	const char* bubbleST = "- press { 1 } for bubble sort";
	const char* selecitonST = "- press { 2 } for selection sort";
	const char* insertionST = "- press { 3 } for insertion sort";
	const char* mergeST = "- press { 4 } for merge sort";
	const char* warningT1 = "WARNING: Giving repetitive commands may cause latency and the visualizer may behave unexpectedly.";
	const char* warningT2 = "Please give a new command only after the current command's execution is done.";

	SDL_Surface* exitS = TTF_RenderText_Solid(font, exitT, { 255,255,255 });
	SDL_Surface* titleS = TTF_RenderText_Solid(font, controlsT, { 255,255,255 });
	SDL_Surface* randomVS = TTF_RenderText_Solid(font, randomVT, { 255, 255, 255 });
	SDL_Surface* bubbleSS = TTF_RenderText_Solid(font, bubbleST, { 255, 255, 255 });
	SDL_Surface* selectionSS = TTF_RenderText_Solid(font, selecitonST, { 255, 255, 255 });
	SDL_Surface* insertionSS = TTF_RenderText_Solid(font, insertionST, { 255, 255, 255 });
	SDL_Surface* mergeSS = TTF_RenderText_Solid(font, mergeST, { 255, 255, 255 });
	SDL_Surface* warningS1 = TTF_RenderText_Solid(font, warningT1, { 255, 0, 0 });
	SDL_Surface* warningS2 = TTF_RenderText_Solid(font, warningT2, { 255, 0, 0 });

	SDL_Texture* exitTexture = SDL_CreateTextureFromSurface(renderer, exitS);
	SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleS);
	SDL_Texture* randomVTexture = SDL_CreateTextureFromSurface(renderer, randomVS);
	SDL_Texture* bubbleSTexture = SDL_CreateTextureFromSurface(renderer, bubbleSS);
	SDL_Texture* selectionSTtexture = SDL_CreateTextureFromSurface(renderer, selectionSS);
	SDL_Texture* insertionSTtexture = SDL_CreateTextureFromSurface(renderer, insertionSS);
	SDL_Texture* mergeSTtexture = SDL_CreateTextureFromSurface(renderer, mergeSS);
	SDL_Texture* warningTexture1 = SDL_CreateTextureFromSurface(renderer, warningS1);
	SDL_Texture* warningTexture2 = SDL_CreateTextureFromSurface(renderer, warningS2);

	SDL_FreeSurface(exitS);
	SDL_FreeSurface(titleS);
	SDL_FreeSurface(randomVS);
	SDL_FreeSurface(bubbleSS);
	SDL_FreeSurface(selectionSS);
	SDL_FreeSurface(insertionSS);
	SDL_FreeSurface(mergeSS);
	SDL_FreeSurface(warningS1);
	SDL_FreeSurface(warningS2);

	SDL_Rect titleR = { 60, 60, 180, 40 };
	SDL_Rect exitSR = { 100, 110, 570, 40 };
	SDL_Rect randomVR = { 100, 145, 860, 40 };
	SDL_Rect bubbleSR = { 100, 180, 530, 40 };
	SDL_Rect selectionSR = { 100, 215, 590, 40 };
	SDL_Rect insertionSR = { 100, 250, 590, 40 };
	SDL_Rect mergeSR = { 100, 285, 520, 40 };
	SDL_Rect warningR1 = { 50, 600, 1100, 25 };
	SDL_Rect warningR2 = { 165, 625, 870, 25 };

	//Border line
	int bd = 40; //borderMargin
	SDL_Rect border = { bd, bd ,SCREEN_WIDTH - 2 * bd, SCREEN_HEIGHT - 2 * bd };
	
	randVector();

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
				case SDLK_r:
					is_finished = false;
					randVector();
					SDL_Delay(1000);
					is_finished = true;
					break;
				case SDLK_1:
					is_finished = false;
					bubbleSort();
					SDL_Delay(1000);
					is_finished = true;
					break;
				case SDLK_2:
					is_finished = false;
					selectionSort();
					SDL_Delay(1000);
					is_finished = true;
					break;
				case SDLK_3:
					is_finished = false;
					insertionSort();
					SDL_Delay(1000);
					is_finished = true;
					break;
				case SDLK_4:
					is_finished = false;
					mergeSort(v, 0, v.size() - 1);
					SDL_Delay(1000);
					is_finished = true;
					break;
				case SDLK_LEFT: // ++ == slower 
					speed++;
					std::cout << "Speed: " << speed << std::endl;
					break;				
				case SDLK_RIGHT: //-- == faster
					if(speed > 0)
						speed--;
					std::cout << "Speed: " << speed << std::endl;
					break;

				}
			}
		}
		//Starting menu
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(renderer, &border);
		SDL_RenderCopy(renderer, exitTexture, NULL, &exitSR);
		SDL_RenderCopy(renderer, titleTexture, NULL, &titleR);
		SDL_RenderCopy(renderer, randomVTexture, NULL, &randomVR);
		SDL_RenderCopy(renderer, bubbleSTexture, NULL, &bubbleSR);
		SDL_RenderCopy(renderer, selectionSTtexture, NULL, &selectionSR);
		SDL_RenderCopy(renderer, insertionSTtexture, NULL, &insertionSR);
		SDL_RenderCopy(renderer, mergeSTtexture, NULL, &mergeSR);
		SDL_RenderCopy(renderer, warningTexture1, NULL, &warningR1);
		SDL_RenderCopy(renderer, warningTexture2, NULL, &warningR2);
		SDL_RenderPresent(renderer);
		}
	}
	close();
		
	return 0;
}