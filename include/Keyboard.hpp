#include <SDL3/SDL.h>
#include <unordered_map>
#include "Chip8.hpp"

class Keyboard{
public:
    void setKeys(Chip8&);
    void init();
    ~Keyboard();

private:
    enum Chip8Key{
        KEY_0 = 0,
        KEY_1 = 1,
        KEY_2 = 2,
        KEY_3 = 3,
        KEY_4 = 4,
        KEY_5 = 5,
        KEY_6 = 6,
        KEY_7 = 7,
        KEY_8 = 8,
        KEY_9 = 9,
        KEY_A = 10,
        KEY_B = 11,
        KEY_C = 12,
        KEY_D = 13,
        KEY_E = 14,
        KEY_F = 15
    };

    const bool* keyStates {nullptr};
    std::unordered_map<SDL_Scancode, Chip8Key> keyMap{};


};