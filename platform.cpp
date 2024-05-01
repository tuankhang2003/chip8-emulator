#include "platform.hpp"
#include <SDL2/SDL.h>

Platform::Platform(char const *title, int windowwidth, int windowHeight, int textureWidth, int textureHight)
{
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(title, 0, 0, windowwidth, windowHeight, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); // REnderer_accelerated: speed up Hardware
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, textureWidth, textureHight);
}
Platform::~Platform()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
void Platform::Update(void const *buffer, int pitch)
{
    SDL_UpdateTexture(texture, NULL, buffer, pitch); // NULL: Update the entire Texture
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}
bool Platform::ProcessInput(uint8_t *key) // KeyDown: KeyPressed   KeyUp: KeyReleased
{
    bool quit = false;
    SDL_Event events;
    while (SDL_PollEvent(&events))
    {
        switch (events.type)
        {
        case SDL_QUIT:
            quit = true;
            break;
        case SDL_KEYDOWN:
        {
            switch (events.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                quit = true;
                break;
            case SDLK_x:
                key[0] = 1;
                break;
            case SDLK_1:
                key[1] = 1;
                break;
            case SDLK_2:
                key[2] = 1;
                break;

            case SDLK_3:
                key[3] = 1;
                break;

            case SDLK_q:
                key[4] = 1;
                break;

            case SDLK_w:
                key[5] = 1;
                break;
            case SDLK_e:
                key[6] = 1;
                break;
            case SDLK_a:
                key[7] = 1;
                break;
            case SDLK_s:
                key[8] = 1;
                break;
            case SDLK_d:
                key[9] = 1;
                break;
            case SDLK_z:
                key[0xA] = 1;
                break;
            case SDLK_c:
                key[0xB] = 1;
                break;
            case SDLK_4:
                key[0xC] = 1;
                break;
            case SDLK_r:
                key[0xD] = 1;
                break;
            case SDLK_f:
                key[0xE] = 1;
                break;
            case SDLK_v:
                key[0xF] = 1;
                break;
            }
        }
        break;

        case SDL_KEYUP:
            switch (events.key.keysym.sym)
            {
            case SDLK_x:
                key[0] = 1;
                break;
            case SDLK_1:
                key[1] = 1;
                break;
            case SDLK_2:
                key[2] = 1;
                break;

            case SDLK_3:
                key[3] = 1;
                break;
            case SDLK_q:
                key[4] = 1;
                break;
            case SDLK_w:
                key[5] = 1;
                break;
            case SDLK_e:
                key[6] = 1;
                break;
            case SDLK_a:
                key[7] = 1;
                break;
            case SDLK_s:
                key[8] = 1;
                break;
            case SDLK_d:
                key[9] = 1;
                break;
            case SDLK_z:
                key[0xA] = 1;
                break;
            case SDLK_c:
                key[0xB] = 1;
                break;
            case SDLK_4:
                key[0xC] = 1;
                break;
            case SDLK_r:
                key[0xD] = 1;
                break;
            case SDLK_f:
                key[0xE] = 1;
                break;
            case SDLK_v:
                key[0xF] = 1;
                break;
            }
            break;
        }
    }
    return quit;
}
