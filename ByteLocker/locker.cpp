#include "locker.h"

void setRandomSeed(char* seed, int len) {

}

unsigned char random() {

}

unsigned char slide(unsigned char a, unsigned char pos) {
	
}

unsigned char deSlide(unsigned char a, unsigned char pos) {
	
}

const int MIX_TURN = 16;
const int RANDOM_USING_COUNT = MIX_TURN * 2;

void byteSpliter(char* data, int size) {
	for (int i = 0; i < size; i++)
		for (int k = 0; k < MIX_TURN; k++) {
			data[i] = slide(data[i], random());
			data[i] = data[i] ^ random();
		}
}

unsigned char randoms[RANDOM_USING_COUNT];
void byteRestorer(char* data, int size) {
	for (int i = 0; i < size; i++) {
		for (int k = 0; k < RANDOM_USING_COUNT; k++)
			randoms[k] = random();
		for (int k = RANDOM_USING_COUNT - 1; k >= 0;) {
			data[i] = data[i] ^ randoms[k--];
			data[i] = deSlide(data[i], randoms[k--]);
		}
	}
}