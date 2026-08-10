#include "../include/Chip8.hpp"
#include <SDL3/SDL.h>

class Renderer {
private:
    const Chip8::DisplayGrid& currentScreen;
    SDL_Window* window{ nullptr };
    SDL_Renderer* renderer {nullptr};

    static constexpr int SCALE {10};

public:
    Renderer(const Chip8::DisplayGrid& chip8Display);
    bool init();
    ~Renderer();
    void render();
};