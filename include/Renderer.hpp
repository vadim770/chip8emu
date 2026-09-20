#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "../include/Chip8.hpp"
#include <SDL3/SDL.h>

class Renderer {
private:
    const Chip8::DisplayGrid& currentScreen;
    SDL_Window* window{ nullptr };
    SDL_Renderer* renderer{ nullptr };

    static constexpr int SCALE {20};
    static constexpr int OFFSET {20};

public:
    Renderer(const Chip8::DisplayGrid& chip8Display);
    ~Renderer();

    bool init();
    void clear();
    void render();
    void present();

    SDL_Window* getWindow() const { return window; }
    SDL_Renderer* getRenderer() const { return renderer; }
};

#endif // RENDERER_HPP
