#include "../include/Chip8.hpp"
#include "../include/Renderer.hpp"

Renderer::Renderer(const Chip8::DisplayGrid& chip8Display) 
    : currentScreen(chip8Display) {}

bool Renderer::init() {
    window = SDL_CreateWindow("CHIP-8 Emulator", int(currentScreen[0].size() * SCALE), int(currentScreen.size() * SCALE), 0);
    
    if (window == nullptr) {
        SDL_Log("Window creation failed! SDL error: %s\n", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        SDL_Log("Renderer creation failed! SDL error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window); 
        window = nullptr;
        return false;
    }

    if (!SDL_SetRenderVSync(renderer, 1)) {
        SDL_Log("Could not enable VSync! SDL error: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

void Renderer::clear() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void Renderer::render() {
    SDL_FRect rect{0.0F, 0.0F, static_cast<float>(SCALE), static_cast<float>(SCALE)};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (size_t y = 0; y < currentScreen.size(); y++) {
        for (size_t x = 0; x < currentScreen[0].size(); x++) {
            if (currentScreen[y][x]) {
                rect.y = static_cast<float>(y * SCALE);
                rect.x = static_cast<float>(x * SCALE);
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}

void Renderer::present() {
    SDL_RenderPresent(renderer);
}

Renderer::~Renderer() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}
