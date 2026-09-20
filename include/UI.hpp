#ifndef UI_HPP
#define UI_HPP

#include <SDL3/SDL.h>
#include <functional>
#include <string>

class UI {
private:
    SDL_Window* window{ nullptr };
    SDL_Renderer* renderer{ nullptr };
    bool showSettingsWindow{ false };
    const char* muteMenuLabel {"Mute"};

public:
    // Callback delegates
    std::function<void(const std::string&)> onROMSelected;
    std::function<void()> onResetSelected;

    std::function<uint64_t()> getIPF;
    std::function<void(uint64_t)> setIPF;

    std::function<void()> muteSound;

    std::function<void()> onExitSelected;

    UI(SDL_Window* sdlWindow, SDL_Renderer* sdlRenderer);
    ~UI();

    bool init();
    void processEvent(const SDL_Event* event);
    void beginFrame();
    void render();
    void endFrame();
};

#endif // UI_HPP
