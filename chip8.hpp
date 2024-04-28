#include <cstdint>
#include <random>
#include <chrono>

const unsigned int START_ADRESSE = 0x200;
const unsigned int FONTSET_SIZE = 16 * 5;
const unsigned int FONTSET_STARTADRESSE = 0x50;
const unsigned int VIDEO_WIDTH = 64;
const unsigned int VIDEO_HEIGHT = 32;
class Chip8
{
public:
    void loadROM(char const *path);
    Chip8();
    void Cycle();
    uint32_t video[64 * 32]; // 64x32 pixel
    uint8_t keypad[16]{};    // 16 input key

private:
    void Table0();
    void Table8();
    void TableE();
    void TableF();
    void OP_NULL();

    void OP_00E0(); // CLS
    void OP_00EE(); // RET
    void OP_1nnn(); // JUMP to nnn
    void OP_2nnn(); // Call to nnn
    void OP_3xkk(); // Skip next Instruction if Vx==kk
    void OP_4xkk(); // SNE Vx, byte
    void OP_5xy0(); // SE Vx, Vy
    void OP_6xkk(); // LD Vx, byte
    void OP_7xkk(); // ADD VX, kk
    void OP_8xy0(); // LD VX VY
    void OP_8xy1(); // OR VX VY
    void OP_8xy2(); // AND VX VY
    void OP_8xy3(); // XOR VX VY
    void OP_8xy4(); // ADD VX VY
    void OP_8xy5(); // SUB VX VY
    void OP_8xy6(); // SHR VX VY  Set Vx=Vx SHR 1 ()
    void OP_8xy7(); //
    void OP_8xyE(); //
    void OP_9xy0(); // SNE Vx Vy
    void OP_Annn(); // LD Index, addr
    void OP_Bnnn(); // JP V0, addr  Jump to V0+addre
    void OP_Cxkk(); // Set Vx= random byte AND kk
    void OP_Dxyn(); // Display n-byte sprite at memory location
    void OP_Ex9E(); // Skip next instruction if key with the value of Vx is pressed.
    void OP_ExA1(); // Skip next instruction if key with the value of Vx is not pressed.
    void OP_Fx07(); // Set Vx = delay timer value.
    void OP_Fx0A(); // Wait for a key press, store the value of the key in Vx.
    void OP_Fx15(); // Set delay timer = Vx.
    void OP_Fx18(); // Set sound timer = Vx.
    void OP_Fx1E(); // ADD Index, Vx
    void OP_Fx29(); // Set I = location of sprite for digit Vx.
    void OP_Fx33(); // Store BCD representation of Vx in memory locations I, I+1, and I+2.
                    // The interpreter takes the decimal value of Vx, and places the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.
    void OP_Fx55(); // Store registers V0 through Vx in memory starting at location I.
    void OP_Fx65(); // Read registers V0 through Vx from memory starting at location I.

    uint8_t registers[16]{}; // 16 REGISTER 8 BIT
    uint8_t memory[4096]{};  // 4KiB MEMORY
    uint16_t index{};        // 16BIT IndexRegister, special register to store memory adresses for use
    uint16_t pc{};
    uint16_t stack[16]{};
    uint8_t sp{};
    uint8_t delaytimer{};
    uint8_t soundtimer{};

    uint16_t opcode;

    std::default_random_engine ranGen;
    std::uniform_int_distribution<int8_t> randByte;

    typedef void (Chip8::*Chip8Func)();
    Chip8Func table[0xF + 1];
    Chip8Func table0[0xE + 1];
    Chip8Func table8[0xE + 1];
    Chip8Func tableE[0xE + 1];
    Chip8Func tableF[0x65 + 1];
};