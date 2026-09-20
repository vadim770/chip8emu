#include <iostream>
#include "../include/Chip8.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "../include/Renderer.hpp"
#include "../include/Keyboard.hpp"
#include "../include/Sound.hpp"
#include "../include/UI.hpp"

int main() {
    constexpr uint64_t FPS = 60;
    constexpr uint64_t FRAME_DELAY = 1000 / FPS; // ~16 ms
    uint64_t INSTRUCTIONS_PER_FRAME = 3;
    Chip8 chip8;
    chip8.initialize();

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
        return -1;
    }

    Renderer renderer(chip8.getDisplay());
    if (!renderer.init()) {
        return -1;
    }

    UI ui(renderer.getWindow(), renderer.getRenderer());
    if (!ui.init()) {
        return -1;
    }

    ui.onROMSelected = [&chip8](const std::string& filepath) {
       chip8.initialize();
       chip8.loadROM(filepath);
   };

   ui.onResetSelected = [&chip8](){
    if(chip8.isLoaded()){
        chip8.initialize();
        chip8.loadROM();
    }
   };

   ui.getIPF = [&INSTRUCTIONS_PER_FRAME]() {
        return INSTRUCTIONS_PER_FRAME;
   };

   ui.setIPF = [&INSTRUCTIONS_PER_FRAME](uint64_t ipf){
        INSTRUCTIONS_PER_FRAME = ipf;
   };

   ui.onExitSelected = [](){
        SDL_Event quitEvent;
        SDL_zero(quitEvent);
        quitEvent.type = SDL_EVENT_QUIT;
        SDL_PushEvent(&quitEvent);
   };


    Sound sound;
    if (!sound.initialize()) {
        return -1;
    }

    ui.muteSound = [&sound](){
        sound.mute();
    };

    Keyboard keyboard;
    keyboard.init();

    SDL_Event e;
    SDL_zero(e);

    bool quit{ false };
    while (!quit) {
        uint64_t frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&e)) {
            ui.processEvent(&e);
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
            if (e.type == SDL_EVENT_WINDOW_MAXIMIZED) {
                
            }
        }
        if (quit) {
            break;
        }


        keyboard.setKeys(chip8);
        for (uint64_t i = 0; i < INSTRUCTIONS_PER_FRAME; ++i) {
            chip8.cycle();
        }

        chip8.decDelayTimer();
        chip8.decSoundTimer();

        if (chip8.getSoundTimer() > 0) {
            sound.play();
        } else {
            sound.stop();
        }

        renderer.clear();
        renderer.render();

        ui.beginFrame();
        ui.render();
        ui.endFrame();

        renderer.present();

        uint64_t frameTime = SDL_GetTicks() - frameStart;
        if (FRAME_DELAY > frameTime) {
            SDL_DelayNS((FRAME_DELAY - frameTime) );
        }
    }

    return 0;
}
