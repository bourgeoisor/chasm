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

const unsigned char boot[] =
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

Emulator::Emulator() {
}

void Emulator::reset() {
	std::cout << "Resetting emulator state..." << std::endl;

	pc = 0x200;
	opcode = 0;
	I = 0;
	sp = 0;

	delayTimer = 0;
	soundTimer = 0;

	for (int i = 0; i < 2048; i++)
		videoBuffer[i] = 0;

	for (int i = 0; i < 16; i++)
		stack[i] = input[i] = V[i] = 0;

	for (int i = 0; i < 4096; i++)
		memory[i] = 0;

	for (int i = 0; i < 80; i++)
		memory[i] = fontset[i];

	// temp boot
	for (int i = 0; i < 133; i++)
		memory[512 + i] = boot[i];
}

void Emulator::cycle() {
	drawFlag = false;

	opcode = memory[pc] << 8 | memory[pc + 1];

	switch (opcode & 0xF000)
	{
	case 0x0000:
		switch (opcode)
		{
		case 0x00E0: // 00E0: Clears the display
			// @todo
			break;
		case 0x00EE: // 00EE: Returns from a subroutine
			// @todo
			break;
		}
		break;
	case 0x1000: // 1NNN: Jumps to the address NNN
		// @todo
		break;
	case 0x2000: // 2NNN: Calls subroutine at NNN
		// @todo
		break;
	case 0x3000: // 3XNN: Skips the next instruction if VX equals NN
		// @todo
		break;
	case 0x4000: // 4XNN: Skips the next instruction if VX doesn't equal NN
		// @todo
		break;
	case 0x5000: // 5XY0: Skips the next instruction if VX equals VY
		// @todo
		break;
	case 0x6000: // 6XNN: Sets VX to NN
		// @todo
		break;
	case 0x7000: // 7XNN: Adds NN to VX
		// @todo
		break;
	case 0x8000:
		switch (opcode & 0x000F) {
		case 0x0000: // 8XY0: Sets VX to the value of VY
			// @todo
			break;
		case 0x0001: // 8XY1: Sets VX to VX or VY
		    // @todo
			break;
		case 0x0002: // 8XY2: Sets VX to VX and VY
		    // @todo
			break;
		case 0x0003: // 8XY3: Sets VX to VX xor VY
		    // @todo
			break;
		case 0x0004: // 8XY4: Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't
		    // @todo
			break;
		case 0x0005: // 8XY5: VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't
		    // @todo
			break;
		case 0x0006: // 8XY6: Stores the least significant bit of VX in VF and then shifts VX to the right by 1
		    // @todo
			break;
		case 0x0007: // 8XY7: Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't
		    // @todo
			break;
		case 0x000E: // 8XYE: Stores the most significant bit of VX in VF and then shifts VX to the left by 1
		    // @todo
			break;
		}
		break;
	case 0x9000: // 9XY0: Skips the next instruction if VX doesn't equal VY
		// @todo
		break;
	case 0xA000: // ANNN: Sets I to the address 
		// @todo
		break;
	case 0xB000: // BNNN: Jumps to the address NNN plus V0
		// @todo
		break;
	case 0xC000: // CXNN: Sets VX to the result of a bitwise and operation on a random number and NN
		// @todo
		break;
	case 0xD000: // DXYN: Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels
		// @todo
		break;
	case 0xE000:
		switch (opcode & 0x00FF) {
		case 0x009E: // EX9E: Skips the next instruction if the key stored in VX is pressed
			// @todo
			break;
		case 0x00A1: // EXA1: Skips the next instruction if the key stored in VX isn't pressed
			// @todo
			break;
		}
		break;
	case 0xF000:
		switch (opcode & 0x00FF) {
		case 0x0007: // FX07: Sets VX to the value of the delay timer
			// @todo
			break;
		case 0x000A: // FX0A: A key press is awaited, and then stored in VX
			// @todo
			break;
		case 0x0015: // FX15: Sets the delay timer to VX
			// @todo
			break;
		case 0x0018: // FX18: Sets the sound timer to VX
			// @todo
			break;
		case 0x001E: // FX1E: Adds VX to I
			// @todo
			break;
		case 0x0029: // FX29: Sets I to the location of the sprite for the character in VX
			// @todo
			break;
		case 0x0033: // FX33: Stores the binary-coded decimal representation of VX, with the most significant of three digits at the address in I
			// @todo
			break;
		case 0x0055: // FX55: Stores V0 to VX (including VX) in memory starting at address I
			// @todo
			break;
		case 0x0065: // FX65: Fills V0 to VX (including VX) with values from memory starting at address I
			// @todo
			break;
		}
		break;
	default:
		std::cout << "Invalid opcode: 0x" << std::hex << std::uppercase << opcode << std::endl;
		// @todo: reset?
	}

	if (delayTimer > 0) delayTimer--;
	if (soundTimer > 0) soundTimer--;
}

bool Emulator::getDrawFlag() {
	return drawFlag;
}

bool* Emulator::getVideoBuffer() {
	return videoBuffer;
}