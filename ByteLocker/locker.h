#pragma once

void setRandomSeed(char* seed, int len);

unsigned char random();

unsigned char slide(unsigned char a, unsigned char pos);

unsigned char deSlide(unsigned char a, unsigned char pos);

void byteSpliter(char* data, int size);

void byteRestorer(char* data, int size);