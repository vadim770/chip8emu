#include <iostream>
#include "../include/Chip8.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "../include/Renderer.hpp"
#include "../include/Keyboard.hpp"
#include "../include/Sound.hpp"

int main(){
    constexpr uint64_t FPS = 60;
    constexpr uint64_t FRAME_DELAY = 1000 / FPS; // ~16 ms
    uint64_t INSTRUCTIONS_PER_FRAME = 5; // might need to change
    Chip8 chip8;
    chip8.initialize();
    

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        SDL_Log( "SDL could not initialize! SDL error: %s\n", SDL_GetError() );
        return -1;
    }

    uint64_t frameStart = SDL_GetTicks();

    Renderer renderer(chip8.getDisplay());
    if(!renderer.init()){
        return -1;
    }

    Sound sound;
    if(!sound.initialize()){
        return -1;
    }

    Keyboard keyboard;
    keyboard.init();


    SDL_Event e;
    SDL_zero( e );

    long long int retSize{chip8.loadROM("../roms/Merlin.ch8")};
    std::cout << "read " << retSize << " bytes\n";
    

    bool quit{ false };
    while(!quit){
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_EVENT_QUIT){
                quit = true;
            }
        }
        if(quit){
            break;
        }

        keyboard.setKeys(chip8);
        for (uint64_t i = 0; i < INSTRUCTIONS_PER_FRAME; ++i) {
            chip8.cycle();
        }

        chip8.decDelayTimer();
        chip8.decSoundTimer();

        if(chip8.getSoundTimer() > 0){
            sound.play();
        }
        else{
            sound.stop();
        }

        renderer.render();

        uint64_t frameTime = SDL_GetTicks() - frameStart;
        if(FRAME_DELAY > frameTime){
            SDL_DelayNS(FRAME_DELAY - frameTime);
        }
    }





    return 0;
}