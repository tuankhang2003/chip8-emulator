#include <SDL2/SDL.h>
#include <chrono>
#include <iostream>
#include "platform.hpp"
#include "chip8.hpp"

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        std::cerr << "Fehler bei TestEingabe";
        std::exit(EXIT_FAILURE);
    }
    int videoScale = std::stoi(argv[1]);
    int cycleDelay = std::stoi(argv[2]);
    char const *ROMFileName = argv[3];
    Platform platform("Chip8-Emulator", videoScale * VIDEO_WIDTH, videoScale * VIDEO_HEIGHT, VIDEO_WIDTH, VIDEO_HEIGHT);
    Chip8 chip8;
    chip8.loadROM(ROMFileName);
    int videoPitch = sizeof(chip8.video[0]) * VIDEO_WIDTH;

    bool quit = false;
    auto lastTimeCycle = std::chrono::high_resolution_clock::now();

    while (!quit)
    {
        quit = platform.ProcessInput(chip8.keypad);
        auto currentTime = std::chrono::high_resolution_clock::now();
        float delta = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastTimeCycle).count();

        if (delta > cycleDelay)
        {
            lastTimeCycle = currentTime;
            chip8.Cycle();
            platform.Update(chip8.video, videoPitch);
        }
    }
    return 0;
}