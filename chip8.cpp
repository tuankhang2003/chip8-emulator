#include "chip8.hpp"
#include <fstream>
#include <iostream>
#include <random>
#include <chrono>
const unsigned int START_ADRESSE = 0x200;
const unsigned int FONTSET_SIZE = 16 * 5;
const unsigned int FONTSET_STARTADRESSE = 0x50;
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

Chip8::Chip8() : ranGen(std::chrono::system_clock::now().time_since_epoch().count())
{

    pc = START_ADRESSE;
    for (unsigned int i = 0; i < FONTSET_SIZE; i++)
    {
        memory[i + FONTSET_STARTADRESSE] = fontset[i];
    }
    randByte = std::uniform_int_distribution<int8_t>(0, 255); // Random Generator
    int s = ranGen(randByte)
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
