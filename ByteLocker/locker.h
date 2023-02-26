#pragma once

unsigned char random();

unsigned char slide(unsigned char a, int pos);

unsigned char deSlide(unsigned char a, int pos);

void byteSpliter(char* data, int size);

void byteRestorer(char* data, int size);