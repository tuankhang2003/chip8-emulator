#include <cstdint>
#include <random>
#include <chrono>
class Chip8
{
public:
    void loadROM(char const *path);
    Chip8();

    uint8_t registers[16]{}; // 16 REGISTER 8 BIT
    uint8_t memory[4096]{};  // 4KiB MEMORY
    uint16_t index{};        // 16BIT IndexRegister, special register to store memory adresses for use
    uint16_t pc{};
    uint16_t stack[16]{};
    uint8_t stackpointer{};
    uint8_t delaytimer{};
    uint8_t soundtimer{};
    uint8_t keypad[16]{};    // 16 input key
    uint32_t video[64 * 32]; // 64x32 pixel
    uint16_t opcode;

    std::default_random_engine ranGen;
    std::uniform_int_distribution<int8_t> randByte;
};