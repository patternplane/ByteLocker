#include "locker.h"
#include <stdio.h>

void setRandomSeed(char* seed, int len) {
}

unsigned char random() {
}

unsigned char slide(unsigned char a, unsigned char pos) {
}

unsigned char deSlide(unsigned char a, unsigned char pos) {
	return slide(a, 8 - (pos % 8));
}

const int MIX_TURN = 16;
const int RANDOM_USING_COUNT = 64;
int running;

void byteSpliter(char* data, int size) {
	unsigned char randomV;
	int randomUsing = 0;
	running = 0;
	randomV = random();
	for (int i = 0; i < size; i++) {
		if (randomUsing == RANDOM_USING_COUNT) {
			randomUsing = 0;
			randomV = random();
		}
		randomV += running++;
		if ((i+1)%1024 == 0)
			printf("%dKB Done\n", (i + 1)/1024);
		for (int k = 0; k < MIX_TURN; k++) {
			data[i] = slide(data[i], randomV + k);
			data[i] = data[i] ^ (randomV + k);
		}
		randomUsing++;
		randomV++;
	}
}

void byteRestorer(char* data, int size) {
	unsigned char randomV;
	int randomUsing = 0;
	running = 0;
	randomV = random();
	for (int i = 0; i < size; i++) {
		if (randomUsing == RANDOM_USING_COUNT) {
			randomUsing = 0;
			randomV = random();
		}
		randomV += running++;
		if ((i + 1) % 1024 == 0)
			printf("%dKB Done\n", (i + 1) / 1024);
		for (int k = MIX_TURN - 1; k >= 0; k--) {
			data[i] = data[i] ^ (randomV + k);
			data[i] = deSlide(data[i], randomV + k);
		}
		randomUsing++;
		randomV++;
	}
}