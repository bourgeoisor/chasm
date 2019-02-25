# Chasm

blah

## Screenshots

blah

## How to Compile

blah

## Specifications

### Memory

blah

### Registers

blah

### Display

blah

### Input

blah

     ┌───┬───┬───┬───┐             ┌───┬───┬───┬───┐
     │ 1 │ 2 │ 3 │ 4 │             │ 1 │ 2 │ 3 │ C │
     └─┬─┴─┬─┴─┬─┴─┬─┴─┐           │───┼───┼───┼───┤
       │ Q │ W │ E │ R │           │ 4 │ 5 │ 6 │ D │
       └─┬─┴─┬─┴─┬─┴─┬─┴─┐   -->   │───┼───┼───┼───┤
         │ A │ S │ D │ F │         │ 7 │ 8 │ 9 │ E │
         └─┬─┴─┬─┴─┬─┴─┬─┴─┐       │───┼───┼───┼───┤
           │ Z │ X │ C │ V │       │ A │ 0 │ B | F |
           └───┴───┴───┴───┘       └───┴───┴───┴───┘

### Timers

There are two timers made available through opcodes:

- A delay timer which decrements by 1 every cycle;
- A sound timer which decrements by 1 every cycle and plays a tone while non-zero;

### Instructions

CHIP-8 has 35 instructions

| Opcode | Type | Description |
|--------|------|-------------|
| 0NNN | Call | Calls RCA 1802 program at address NNN. |
| 00E0 | Display | Clears the screen. |
| 00EE | Flow | Returns from a subroutine. |
| 1NNN | Flow | Jumps to address NNN. |
| 2NNN | Flow | Calls subroutine at NNN. |
| 3XNN | Cond | Skips the next instruction if VX equals NN. |
| 4XNN | Cond | Skips the next instruction if VX doesn't equal NN. |
| 5XY0 | Cond | Skips the next instruction if VX equals VY. |
| 6XNN | Const | Sets VX to NN. |
| 7XNN | Const | Adds NN to VX. |
| 8XY0 | Assign | Sets VX to the value of VY. |
| 8XY1 | BitOp | Sets VX to VX OR VY. |
| 8XY2 | BitOp | Sets VX to VX AND VY. |
| 8XY3 | BitOp | Sets VX to VX XOR VY. |
| 8XY4 | Math | Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't. |
| 8XY5 | Math | VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't. |
| 8XY6 | BitOp | Stores the least significant bit of VX in VF and then shifts VX to the right by 1. |
| 8XY7 | Math | Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't. |
| 8XYE | BitOp | Stores the most significant bit of VX in VF and then shifts VX to the left by 1. |
| 9XY0 | Cond | Skips the next instruction if VX doesn't equal VY. |
| ANNN | Mem | Sets I to the address NNN. |
| BNNN | Flow | Jumps to the address NNN plus V0. |
| CXNN | Rand | Sets VX to the result of a bitwise and operation on a random number and NN. |
| DXYN | Disp | Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. |
| EX9E | KeyOp | Skips the next instruction if the key stored in VX is pressed. |
| EXA1 | KeyOp | Skips the next instruction if the key stored in VX isn't pressed. |
| FX07 | Timer | Sets VX to the value of the delay timer. |
| FX0A | KeyOp | A key press is awaited, and then stored in VX. |
| FX15 | Timer | Sets the delay timer to VX. |
| FX18 | Sound | Sets the sound timer to VX. |
| FX1E | Mem | Adds VX to I. |
| FX29 | Mem | Sets I to the location of the sprite for the character in VX. |
| FX33 | Mem | Stores the binary-coded decimal representation of VX at address I through I+2. |
| FX55 | Mem | Stores V0 to VX (including VX) in memory starting at address I. |
| FX65 | Mem | Fills V0 to VX (including VX) with values from memory starting at address I. |

## Development State

Chasm is currently fully working, but I may plan on adding a few little features here and there. For any contribution ideas or bug reports, please feel free to either message me for more information or to directly send in a pull request.