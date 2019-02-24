#include <iostream>
#include "emulator.h"

const unsigned char fontset[80] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0,
	0x20, 0x60, 0x20, 0x20, 0x70,
	0xF0, 0x10, 0xF0, 0x80, 0xF0,
	0xF0, 0x10, 0xF0, 0x10, 0xF0,
	0x90, 0x90, 0xF0, 0x10, 0x10,
	0xF0, 0x80, 0xF0, 0x10, 0xF0,
	0xF0, 0x80, 0xF0, 0x90, 0xF0,
	0xF0, 0x10, 0x20, 0x40, 0x40,
	0xF0, 0x90, 0xF0, 0x90, 0xF0,
	0xF0, 0x90, 0xF0, 0x10, 0xF0,
	0xF0, 0x90, 0xF0, 0x90, 0x90,
	0xE0, 0x90, 0xE0, 0x90, 0xE0,
	0xF0, 0x80, 0x80, 0x80, 0xF0,
	0xE0, 0x90, 0x90, 0x90, 0xE0,
	0xF0, 0x80, 0xF0, 0x80, 0xF0,
	0xF0, 0x80, 0xF0, 0x80, 0x80
};

const unsigned char boot[133] =
{
	0xA2, 0x5B, 0x60, 0x0B, 0x61, 0x03, 0x62, 0x07,
	0xD0, 0x17, 0x70, 0x07, 0xF2, 0x1E, 0xD0, 0x17,
	0x70, 0x07, 0xF2, 0x1E, 0xD0, 0x17, 0x70, 0x07,
	0xF2, 0x1E, 0xD0, 0x17, 0x70, 0x07, 0xF2, 0x1E,
	0xD0, 0x17, 0x70, 0x05, 0xF2, 0x1E, 0xD0, 0x17,
	0xF2, 0x1E, 0xA2, 0x5A, 0xC0, 0x3F, 0xC1, 0x1F,
	0x62, 0x01, 0x63, 0x01, 0xD0, 0x11, 0x64, 0x02,
	0xF4, 0x15, 0xF4, 0x07, 0x34, 0x00, 0x12, 0x3A,
	0xD0, 0x11, 0x80, 0x24, 0x81, 0x34, 0xD0, 0x11,
	0x41, 0x00, 0x63, 0x01, 0x41, 0x1F, 0x63, 0xFF,
	0x40, 0x00, 0x62, 0x01, 0x40, 0x3F, 0x62, 0xFF,
	0x12, 0x36, 0x80, 0x78, 0xCC, 0xC0, 0xC0, 0xC0,
	0xCC, 0x78, 0xCC, 0xCC, 0xCC, 0xFC, 0xCC, 0xCC,
	0xCC, 0xFC, 0x30, 0x30, 0x30, 0x30, 0x30, 0xFC,
	0xF8, 0xCC, 0xCC, 0xF8, 0xC0, 0xC0, 0xC0, 0x00,
	0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x78, 0xCC,
	0xCC, 0x78, 0xCC, 0xCC, 0x78,
};

Emulator::Emulator(Input input) : input(input) {
}

void Emulator::reset() {
	std::cout << "Resetting emulator state..." << std::endl;

	pc = 0x200;
	I = 0;
	sp = 0;

	delayTimer = 0;
	soundTimer = 0;

	std::fill(videoBuffer, videoBuffer + 2048, 0);
	std::fill(stack, stack + 16, 0);
	std::fill(V, V + 16, 0);

	std::fill(memory, memory + 4096, 0);

	for (int i = 0; i < 80; i++)
		memory[i] = fontset[i];

	// temp boot
	// @todo: only load if no rom loaded
	for (int i = 0; i < 133; i++)
		memory[512 + i] = boot[i];
}

void Emulator::load(std::vector<unsigned char> buffer) {
	for (int i = 0; i < buffer.size(); i++)
		memory[512 + i] = buffer[i];
}

void Emulator::cycle() {
	drawFlag = false;

	unsigned short opcode = memory[pc] << 8 | memory[pc + 1];
	pc += 2;

	switch (opcode & 0xF000)
	{
	case 0x0000:
		switch (opcode)
		{
		case 0x00E0: // 00E0: Clears the display
			std::fill(videoBuffer, videoBuffer + 2048, 0);
			drawFlag = true;
			break;
		case 0x00EE: // 00EE: Returns from a subroutine
			sp--;
			pc = stack[sp];
			break;
		default: // 0NNN: Calls RCA 1802 program at address NNN
			std::cout << "Syscall not implemented: 0x" << std::hex << std::uppercase << opcode << std::endl;
		}
		break;
	case 0x1000: // 1NNN: Jumps to the address NNN
		pc = opcode & 0x0FFF;
		break;
	case 0x2000: // 2NNN: Calls subroutine at NNN
		stack[sp] = pc;
		sp++;
		pc = opcode & 0x0FFF;
		break;
	case 0x3000: // 3XNN: Skips the next instruction if VX equals NN
		if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) pc += 2;
		break;
	case 0x4000: // 4XNN: Skips the next instruction if VX doesn't equal NN/
		if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) pc += 2;
		break;
	case 0x5000: // 5XY0: Skips the next instruction if VX equals VY
		if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4]) pc += 2;
		break;
	case 0x6000: // 6XNN: Sets VX to NN
		V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
		break;
	case 0x7000: // 7XNN: Adds NN to VX
		V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
		break;
	case 0x8000:
		switch (opcode & 0x000F) {
		case 0x0000: // 8XY0: Sets VX to the value of VY
			V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
			break;
		case 0x0001: // 8XY1: Sets VX to VX or VY
			V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
			break;
		case 0x0002: // 8XY2: Sets VX to VX and VY
			V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
			break;
		case 0x0003: // 8XY3: Sets VX to VX xor VY
			V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
			break;
		case 0x0004: // 8XY4: Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't
			V[0xF] = V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8]);
			V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
			break;
		case 0x0005: // 8XY5: VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't
			V[0xF] = V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4];
			V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
			break;
		case 0x0006: // 8XY6: Stores the least significant bit of VX in VF and then shifts VX to the right by 1
			V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x1;
			V[(opcode & 0x0F00) >> 8] >>= 1;
			break;
		case 0x0007: // 8XY7: Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't
			V[0xF] = V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8];
			V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
			break;
		case 0x000E: // 8XYE: Stores the most significant bit of VX in VF and then shifts VX to the left by 1
			V[0xF] = (V[(opcode & 0x0F00) >> 8] & 0x8000) >> 15;
			V[(opcode & 0x0F00) >> 8] <<= 1;
			break;
		}
		break;
	case 0x9000: // 9XY0: Skips the next instruction if VX doesn't equal VY
		if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]) pc += 2;
		break;
	case 0xA000: // ANNN: Sets I to the address 
		I = opcode & 0x0FFF;
		break;
	case 0xB000: // BNNN: Jumps to the address NNN plus V0
		pc = (opcode & 0x0FFF) + V[0x0];
		break;
	case 0xC000: // CXNN: Sets VX to the result of a bitwise and operation on a random number and NN
		V[(opcode & 0x0F00) >> 8] = (std::rand() % 0xFF) & (opcode & 0x00FF);
		break;
	case 0xD000: // DXYN: Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels
		V[0xF] = 0;
		for (int j = 0; j < (opcode & 0x000F); j++) {
			unsigned short pixel = memory[I + j];
			for (int i = 0; i < 8; i++) {
				if ((pixel & (0x80 >> i)) != 0) {
					if (videoBuffer[(V[(opcode & 0x0F00) >> 8] + i + ((V[(opcode & 0x00F0) >> 4] + j) * 64))] == 1) V[0xF] = 1;
					videoBuffer[V[(opcode & 0x0F00) >> 8] + i + ((V[(opcode & 0x00F0) >> 4] + j) * 64)] ^= 1;
				}
			}
		}
		drawFlag = true;
		break;
	case 0xE000:
		switch (opcode & 0x00FF) {
		case 0x009E: // EX9E: Skips the next instruction if the key stored in VX is pressed
			if (input.isKeyPressed(V[(opcode & 0x0F00) >> 8])) pc += 2;
			break;
		case 0x00A1: // EXA1: Skips the next instruction if the key stored in VX isn't pressed
			if (!input.isKeyPressed(V[(opcode & 0x0F00) >> 8])) pc += 2;
			break;
		}
		break;
	case 0xF000:
		switch (opcode & 0x00FF) {
		case 0x0007: // FX07: Sets VX to the value of the delay timer
			V[(opcode & 0x0F00) >> 8] = delayTimer;
			break;
		case 0x000A: // FX0A: A key press is awaited, and then stored in VX
			// @todo
			V[(opcode & 0x0F00) >> 8] = 0;
			break;
		case 0x0015: // FX15: Sets the delay timer to VX
			delayTimer = V[(opcode & 0x0F00) >> 8];
			break;
		case 0x0018: // FX18: Sets the sound timer to VX
			soundTimer = V[(opcode & 0x0F00) >> 8];
			break;
		case 0x001E: // FX1E: Adds VX to I
			I += V[(opcode & 0x0F00) >> 8];
			break;
		case 0x0029: // FX29: Sets I to the location of the sprite for the character in VX
			I = V[(opcode & 0x0F00) >> 8] * 5;
			break;
		case 0x0033: // FX33: Stores the binary-coded decimal representation of VX, with the most significant of three digits at the address in I
			memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
			memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
			memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
			break;
		case 0x0055: // FX55: Stores V0 to VX (including VX) in memory starting at address I
			for (int i = 0; i < (opcode & 0x0F00) >> 8; i++)
				memory[I + i] = V[i];
			break;
		case 0x0065: // FX65: Fills V0 to VX (including VX) with values from memory starting at address I
			for (int i = 0; i < (opcode & 0x0F00) >> 8; i++)
				V[i] = memory[I + i];
			break;
		}
		break;
	default:
		std::cout << "Invalid opcode: 0x" << std::hex << std::uppercase << opcode << std::endl;
		// @todo: panic
		// @todo: defaults on all sub-switches?
	}

	if (delayTimer > 0) delayTimer--;
	if (soundTimer > 0) soundTimer--;
}

bool Emulator::getDrawFlag() {
	return drawFlag;
}

unsigned short Emulator::getSoundTimer() {
	return soundTimer;
}

bool* Emulator::getVideoBuffer() {
	return videoBuffer;
}