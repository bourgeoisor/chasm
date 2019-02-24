#ifndef EMULATOR_H
#define EMULATOR_H

#include <vector>
#include "input.h"

class Emulator {
public:
	Emulator(Input input);
	void reset();
	void load(std::vector<unsigned char> buffer);
	void cycle();
	bool getDrawFlag();
	unsigned short getSoundTimer();
	bool* getVideoBuffer();

private:
	Input input;

	unsigned char memory[4096];
	unsigned short pc;

	unsigned char V[16];
	unsigned short I;

	unsigned short stack[16];
	unsigned short sp;

	bool drawFlag;
	bool videoBuffer[64 * 32];
	unsigned char delayTimer;
	unsigned char soundTimer;
};

#endif