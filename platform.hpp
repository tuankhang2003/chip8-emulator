#include <SDL2/SDL.h>

class Platform
{
public:
    Platform(char const *title, int windowwidth, int windowHeight, int textureWidth, int textureHight);
    ~Platform();
    void Update(void const *buffer, int pitch);
    bool ProcessInput(uint8_t *keys);

private:
    SDL_Window *window{};
    SDL_GLContext gl_context{};
    SDL_Renderer *renderer{};
    SDL_Texture *texture{};
};
