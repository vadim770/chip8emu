#include "../include/Keyboard.hpp"

void Keyboard::init(){
    keyStates = SDL_GetKeyboardState(NULL);
    // setting the default key layout
    keyMap[SDL_SCANCODE_1] = KEY_1;
    keyMap[SDL_SCANCODE_2] = KEY_2;
    keyMap[SDL_SCANCODE_3] = KEY_3;
    keyMap[SDL_SCANCODE_4] = KEY_C;
    keyMap[SDL_SCANCODE_Q] = KEY_4;
    keyMap[SDL_SCANCODE_W] = KEY_5;
    keyMap[SDL_SCANCODE_E] = KEY_6;
    keyMap[SDL_SCANCODE_R] = KEY_D;
    keyMap[SDL_SCANCODE_A] = KEY_7;
    keyMap[SDL_SCANCODE_S] = KEY_8;
    keyMap[SDL_SCANCODE_D] = KEY_9;
    keyMap[SDL_SCANCODE_F] = KEY_E;
    keyMap[SDL_SCANCODE_Z] = KEY_A;
    keyMap[SDL_SCANCODE_X] = KEY_0;
    keyMap[SDL_SCANCODE_C] = KEY_B;
    keyMap[SDL_SCANCODE_V] = KEY_F;
    
}

void Keyboard::setKeys(Chip8& chip8){
    for(const std::pair<const SDL_Scancode, Chip8Key>& pair : keyMap){
        if(keyStates[pair.first]){
            chip8.setKey(static_cast<uint8_t>(pair.second));
        }

        else{
            chip8.unsetKey(static_cast<uint8_t>(pair.second));
        }
    }
}


Keyboard::~Keyboard(){
    keyStates = nullptr;
}