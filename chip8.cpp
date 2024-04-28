#include "chip8.hpp"
#include <fstream>
#include <iostream>
#include <random>
#include <chrono>
#include <cstring>

uint8_t fontset[FONTSET_SIZE] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8::Chip8() : ranGen(std::chrono::system_clock::now().time_since_epoch().count()) // to generate random (seed)
{

    pc = START_ADRESSE;
    for (unsigned int i = 0; i < FONTSET_SIZE; i++)
    {
        memory[i + FONTSET_STARTADRESSE] = fontset[i];
    }
    randByte = std::uniform_int_distribution<int8_t>(0, 255); // Random Generator

    table[0x0] = &Chip8::Table0;
    table[0x1] = &Chip8::OP_1nnn;
    table[0x2] = &Chip8::OP_2nnn;
    table[0x3] = &Chip8::OP_3xkk;
    table[0x4] = &Chip8::OP_4xkk;
    table[0x5] = &Chip8::OP_5xy0;
    table[0x6] = &Chip8::OP_6xkk;
    table[0x7] = &Chip8::OP_7xkk;
    table[0x8] = &Chip8::Table8;
    table[0x9] = &Chip8::OP_9xy0;
    table[0xA] = &Chip8::OP_Annn;
    table[0xB] = &Chip8::OP_Bnnn;
    table[0xC] = &Chip8::OP_Cxkk;
    table[0xD] = &Chip8::OP_Dxyn;
    table[0xE] = &Chip8::TableE;
    table[0xF] = &Chip8::TableF;

    for (unsigned int i = 0; i <= 0xE; i++)
    {
        table0[i] = &Chip8::OP_NULL;
        table8[i] = &Chip8::OP_NULL;
        tableE[i] = &Chip8::OP_NULL;
    }
    table0[0x0] = &Chip8::OP_00E0;
    table0[0xE] = &Chip8::OP_00EE;

    table8[0x0] = &Chip8::OP_8xy0;
    table8[0x1] = &Chip8::OP_8xy1;
    table8[0x2] = &Chip8::OP_8xy2;
    table8[0x3] = &Chip8::OP_8xy3;
    table8[0x4] = &Chip8::OP_8xy4;
    table8[0x5] = &Chip8::OP_8xy5;
    table8[0x6] = &Chip8::OP_8xy6;
    table8[0x7] = &Chip8::OP_8xy7;
    table8[0xE] = &Chip8::OP_8xyE;

    tableE[0x1] = &Chip8::OP_ExA1;
    tableE[0xE] = &Chip8::OP_Ex9E;

    for (unsigned int i = 0; i <= 0x65; i++)
    {
        tableF[i] = &Chip8::OP_NULL;
    }
    tableF[0x07] = &Chip8::OP_Fx07;
    tableF[0x0A] = &Chip8::OP_Fx0A;
    tableF[0x15] = &Chip8::OP_Fx15;
    tableF[0x18] = &Chip8::OP_Fx18;
    tableF[0x1E] = &Chip8::OP_Fx1E;
    tableF[0x29] = &Chip8::OP_Fx29;
    tableF[0x33] = &Chip8::OP_Fx33;
    tableF[0x55] = &Chip8::OP_Fx55;
    tableF[0x65] = &Chip8::OP_Fx65;
}

void Chip8::Cycle()
{
    // Fetch Intruction (2 Byte)
    opcode = (memory[pc] << 8u) | memory[pc + 1];
    pc += 2;
    // Decode and execute
    ((*this).*(table[(opcode & 0xF000u) >> 12u]))();

    // Decrement soundtimer
    if (soundtimer > 0)
    {
        soundtimer--;
    }
    // Decrement delaytimer
    if (delaytimer > 0)
    {
        delaytimer--;
    }
}
void Chip8::loadROM(char const *path)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (file.is_open())
    {
        std::streampos fsize = file.tellg();
        char *buffer = new char[fsize];
        file.seekg(0, std::ios::beg); // return the beginning of the stream
        file.read(buffer, fsize);
        if (!file)
        {
            std::cout << "Error: only " << file.gcount() << " could be read";
        }
        file.close();
        for (long i = 0; i < fsize; i++)
        {
            memory[START_ADRESSE + i] = buffer[i];
        }
        delete[] buffer;
    }
}

void Chip8::OP_00E0() // Clear the Display (CLS)
{
    memset(video, 0, sizeof(video));
}
void Chip8::OP_00EE() // RET
{
    pc = stack[--sp];
}
void Chip8::OP_1nnn() // JUMP TO nnn
{
    uint16_t address = opcode & 0xFFFU;
    pc = address;
}

void Chip8::OP_2nnn() // CALL TO nnn
{
    uint16_t address = opcode & 0xFFFU;
    stack[sp++] = pc;
    pc = address;
}
void Chip8::OP_3xkk() // SKIP NEXT Instruction if Vx==kk  (SE Vx kk)
{
    uint8_t Vx = (opcode & 0x0F00U) >> 8U;
    uint8_t kk = (opcode & 0x00FFU);
    if (registers[Vx] == kk)
    {
        pc += 2;
    }
}
void Chip8::OP_4xkk() // SNE Vx kk
{
    uint8_t Vx = (opcode & 0x0F00U) >> 8U;
    uint8_t kk = (opcode & 0x00FFU);
    if (registers[Vx] != kk)
    {
        pc += 2;
    }
}
void Chip8::OP_5xy0() // SE Vx Vy
{
    uint8_t Vx = (opcode & 0x0F00U) >> 8U;
    uint8_t Vy = (opcode & 0x00F0U) >> 4U;
    if (registers[Vx] == registers[Vy])
    {
        pc += 2;
    }
}
void Chip8::OP_6xkk() // LD Vx, byte
{
    uint8_t Vx = (opcode & 0x0F00U) >> 8U;
    uint8_t byte = (opcode & 0x00FFU);
    registers[Vx] = memory[byte];
}
void Chip8::OP_7xkk() // ADD VX, byte
{
    uint8_t Vx = (opcode & 0x0F00U) >> 8U;
    uint8_t byte = (opcode & 0x00FFU);
    registers[Vx] += byte;
}
void Chip8::OP_8xy0() // LD VX VY
{
    uint8_t Vx = (opcode & 0x0F00U) >> 8U;
    uint8_t Vy = (opcode & 0x00F0U) >> 4U;
    registers[Vx] = registers[Vy];
}
void Chip8::OP_8xy1() // OR VX VY
{
    uint8_t Vx = (opcode & 0x0F00U) >> 8U;
    uint8_t Vy = (opcode & 0x00F0U) >> 4U;
    registers[Vx] |= registers[Vy];
}
void Chip8::OP_8xy2() // AND VX VY
{
    uint8_t Vx = (opcode & 0x0F00U) >> 8U;
    uint8_t Vy = (opcode & 0x00F0U) >> 4U;
    registers[Vx] &= registers[Vy];
}
void Chip8::OP_8xy3() // XOR VX VY
{
    uint8_t Vx = (opcode & 0x0F00U) >> 8U;
    uint8_t Vy = (opcode & 0x00F0U) >> 4U;
    registers[Vx] ^= registers[Vy];
}
void Chip8::OP_8xy4() // ADD VX VY  Set Vx = Vx + Vy, set VF = carry.

// The values of Vx and Vy are added together. If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx.
{
    uint8_t Vx = (opcode & 0x0F00U) >> 8U;
    uint8_t Vy = (opcode & 0x00F0U) >> 4U;
    uint16_t sum = registers[Vx] + registers[Vy];
    if (sum > 255U)
    {
        registers[0xF] = 1;
    }
    else
    {
        registers[0xF] = 0;
    }
    registers[Vx] = sum & 0xFFU;
}
void Chip8::OP_8xy5() // SUB VX VY
{
    uint8_t Vx = (opcode & 0x0F00U) >> 8U;
    uint8_t Vy = (opcode & 0x00F0U) >> 4U;
    if (registers[Vx] > registers[Vy])
    {
        registers[0xF] = 1;
    }
    else
    {
        registers[0xF] = 0;
    }
    registers[Vx] -= registers[Vy];
}
void Chip8::OP_8xy6() // SHR Vx
{
    uint8_t Vx = (opcode & 0x0F00U) >> 8U;
    registers[0xF] = registers[Vx] & (0x1U);
    registers[Vx] >>= 1;
}
void Chip8::OP_8xy7() // SUBN VX VY
{
    uint8_t Vx = (opcode & 0x0F00U) >> 8U;
    uint8_t Vy = (opcode & 0x00F0U) >> 4U;
    if (registers[Vx] > registers[Vy])
    {
        registers[0xF] = 1;
    }
    else
    {
        registers[0xF] = 0;
    }
    registers[Vx] = registers[Vy] - registers[Vx];
}
void Chip8::OP_8xyE()
{
    uint8_t Vx = (opcode & 0x0F00U) >> 8U;
    registers[0xF] = registers[Vx] & (0x80U) >> 7U;
    registers[Vx] >>= 1;
}
void Chip8::OP_9xy0()
{
    uint8_t Vx = (opcode & 0x0F00U) >> 8U;
    uint8_t Vy = (opcode & 0x00F0U) >> 4U;
    if (registers[Vx] != registers[Vy])
    {
        pc += 2;
    }
}
void Chip8::OP_Annn()
{
    uint8_t address = (opcode & 0x0FFFU);
    index = address;
}
void Chip8::OP_Bnnn()
{
    uint8_t address = (opcode & 0x0FFFU);
    pc = registers[0] + address;
}
void Chip8::OP_Cxkk()
{
    uint8_t Vx = (opcode & 0x0F00U) >> 8U;
    uint8_t byte = (opcode & 0x00FFU);
    registers[Vx] = randByte(ranGen) & byte;
}
void Chip8::OP_Dxyn()
{
    uint8_t Vx = (opcode & 0x0F00U) >> 8U;
    uint8_t Vy = (opcode & 0x00F0U) >> 4U;
    uint8_t height = (opcode & 0x000FU);

    uint8_t xPos = registers[Vx] % VIDEO_WIDTH;
    uint8_t yPos = registers[Vy] % VIDEO_HEIGHT;

    registers[0xF] = 0;
    for (unsigned int row = 0; row < height; row++)
    {
        uint8_t byte = memory[row + index];
        for (unsigned int col = 0; col < 8; col++)
        {
            uint8_t pixel = byte & (0x80U >> col);
            uint32_t *screenPixel = &video[(row + yPos) * VIDEO_WIDTH + col + xPos];
            if (pixel)
            {
                if (*screenPixel == 0xFFFFFFFF) // if screenPixel is on
                {
                    registers[0xF] = 1;
                }
            }
            *screenPixel ^= 0xFFFFFFFF;
        }
    }
}
void Chip8::OP_Ex9E() // SKP Vx
{
    uint8_t Vx = (opcode & 0x0F00U) >> 8U;
    if (keypad[registers[Vx]])
    {
        pc += 2;
    }
}
void Chip8::OP_ExA1() // SKNP Vx
{
    uint8_t Vx = (opcode & 0x0F00U) >> 8U;
    if (!keypad[registers[Vx]])
    {
        pc += 2;
    }
}
void Chip8::OP_Fx07()
{
    uint8_t Vx = (opcode & 0x0F00U) >> 8U;
    registers[Vx] = delaytimer;
}
void Chip8::OP_Fx0A() // easiest way: pc-2
{
    uint8_t Vx = (opcode & 0x0F00U) >> 8U;
    if (keypad[0])
    {
        registers[Vx] = 0;
    }
    else if (keypad[1])
    {
        registers[Vx] = 1;
    }
    else if (keypad[2])
    {
        registers[Vx] = 2;
    }
    else if (keypad[3])
    {
        registers[Vx] = 3;
    }
    else if (keypad[4])
    {
        registers[Vx] = 4;
    }
    else if (keypad[5])
    {
        registers[Vx] = 5;
    }
    else if (keypad[6])
    {
        registers[Vx] = 6;
    }
    else if (keypad[7])
    {
        registers[Vx] = 7;
    }
    else if (keypad[8])
    {
        registers[Vx] = 8;
    }
    else if (keypad[9])
    {
        registers[Vx] = 9;
    }
    else if (keypad[10])
    {
        registers[Vx] = 10;
    }
    else if (keypad[11])
    {
        registers[Vx] = 11;
    }
    else if (keypad[12])
    {
        registers[Vx] = 12;
    }
    else if (keypad[13])
    {
        registers[Vx] = 13;
    }
    else if (keypad[14])
    {
        registers[Vx] = 14;
    }
    else if (keypad[15])
    {
        registers[Vx] = 15;
    }
    else
    {
        pc -= 2;
    }
}
void Chip8::OP_Fx15()
{
    uint8_t Vx = (opcode & 0x0F00U) >> 8U;
    delaytimer = registers[Vx];
}
void Chip8::OP_Fx18()
{
    uint8_t Vx = (opcode & 0x0F00U) >> 8U;
    soundtimer = registers[Vx];
}
void Chip8::OP_Fx1E()
{
    uint8_t Vx = (opcode & 0x0F00U) >> 8U;
    index += registers[Vx];
}
void Chip8::OP_Fx29() // each number hat 5 byte -> z.B. access 2 -> Startadress+ 5*2
{
    uint8_t Vx = (opcode & 0x0F00U) >> 8U;
    uint8_t digit = registers[Vx];
    index = FONTSET_STARTADRESSE + digit * 5;
}
void Chip8::OP_Fx33()
{
    uint8_t Vx = (opcode & 0x0F00U) >> 8U;
    uint8_t value = registers[Vx];
    // One-place
    memory[index + 2] = value % 10;
    value /= 10;
    // Ten-place
    memory[index + 1] = value % 10;
    value /= 10;
    // Hundred-place
    memory[index] = value % 10;
}
void Chip8::OP_Fx55()
{
    uint8_t Vx = (opcode & 0x0F00U) >> 8U;
    for (uint8_t i = 0; i <= Vx; i++)
    {
        memory[i + index] = registers[i];
    }
}
void Chip8::OP_Fx65()
{
    uint8_t Vx = (opcode & 0x0F00U) >> 8U;
    for (uint8_t i = 0; i <= Vx; i++)
    {
        registers[i] = memory[i + index];
    }
}
void Chip8::Table0()
{
    ((*this).*(table0[opcode & 0x000Fu]))();
}
void Chip8::Table8()
{
    ((*this).*(table8[opcode & 0x000FU]))();
}
void Chip8::TableE()
{
    ((*this).*(tableE[opcode & 0x000FU]))();
}
void Chip8::TableF()
{
    ((*this).*(tableF[opcode & 0x00FFU]))();
}
void Chip8::OP_NULL()
{
}