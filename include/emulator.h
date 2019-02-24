#ifndef EMULATOR_H
#define EMULATOR_H

class Emulator {
public:
	Emulator();
	void reset();
	void cycle();
	bool getDrawFlag();
	bool* getVideoBuffer();

private:
	unsigned short opcode;
	unsigned char memory[4096];
	unsigned char V[16];
	unsigned short I;
	unsigned short pc;
	unsigned short stack[16];
	unsigned short sp;

	bool input[16];
	bool drawFlag;
	bool videoBuffer[64 * 32];
	unsigned char delayTimer;
	unsigned char soundTimer;
};

#endif