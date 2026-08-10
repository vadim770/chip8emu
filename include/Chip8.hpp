#pragma once
#include <unordered_map>
#include <cstdint>
#include <string_view>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <random>
#include <array>
#include <SDL3/SDL.h>

class Chip8 {
private:
    static constexpr int DISPLAY_WIDTH {64};
    static constexpr int DISPLAY_HEIGHT {32};

public:
    void initialize();
    long long int loadROM(std::string_view);
    void cycle();
    using DisplayGrid = std::array<std::array<bool, DISPLAY_WIDTH>, DISPLAY_HEIGHT>;
    const DisplayGrid& getDisplay() const;
    void setKey(uint8_t);
    void unsetKey(uint8_t);
    void decDelayTimer();
    void decSoundTimer();
    const uint8_t& getSoundTimer() const;


private:
    // Constants
    static constexpr int MEMORY_SIZE {4096};
    static constexpr int REGISTER_COUNT {16};
    static constexpr int STACK_SIZE {16};
    static constexpr int FONT_SIZE {80};
    static constexpr int MAX_FILE_SIZE {3584};
    static constexpr int KEY_PAD_SIZE {16};
    static constexpr int PIXEL_WIDTH {8};

    static constexpr uint16_t CALL{0x0000};
    static constexpr uint16_t CLR_DISPLAY{0x00E0};
    static constexpr uint16_t RETURN_FROM_ROUTINE{0x00EE};
    static constexpr uint16_t JUMP_TO_NNN{0x1000};
    static constexpr uint16_t CALL_ROUTINE{0x2000};
    static constexpr uint16_t IF_VX_EQ_NN{0x3000};
    static constexpr uint16_t IF_VX_NOT_NN{0x4000};
    static constexpr uint16_t IF_VX_EQ_VY{0x5000};
    static constexpr uint16_t SET_VX_TO_NN{0x6000};
    static constexpr uint16_t ADD_NN_TO_VX{0x7000};
    static constexpr uint16_t SET_VX_TO_VY{0x8000};
    static constexpr uint16_t SET_VX_TO_VX_OR_VY{0x8001};
    static constexpr uint16_t SET_VX_TO_VX_AND_VY{0x8002};
    static constexpr uint16_t SET_VX_TO_VX_XOR_VX{0x8003};
    static constexpr uint16_t ADD_VY_TO_VX{0x8004};
    static constexpr uint16_t SUB_VY_FROM_VX{0x8005};
    static constexpr uint16_t SHIFT_VX_RIGHT{0x8006};
    static constexpr uint16_t SUB_VX_FROM_VY{0x8007};
    static constexpr uint16_t SHIFT_VX_LEFT{0x8008};
    static constexpr uint16_t IF_VX_NOTEQ_VY{0x9000};
    static constexpr uint16_t SET_I_TO_NNN{0xA000};
    static constexpr uint16_t JUMP_TO_NNN_PLUS_V0{0xB000};
    static constexpr uint16_t SET_VX_RANDOM{0xC000};
    static constexpr uint16_t DRAW{0xD000};
    static constexpr uint16_t IF_KEY_EQ_VX{0xE09E};
    static constexpr uint16_t IF_KEY_NOT_EQ_VX{0xE0A1};
    static constexpr uint16_t SET_VX_TO_DELAY{0xF007};
    static constexpr uint16_t SET_VX_TO_KEY{0xF00A};
    static constexpr uint16_t SET_DELAY_TO_VX{0xF015};
    static constexpr uint16_t SET_SOUND_TO_VX{0x0F018};
    static constexpr uint16_t ADD_VX_TO_I{0xF01E};
    static constexpr uint16_t SET_I_TO_SPRITE_ADDR{0xF029};
    static constexpr uint16_t STORE_BCD{0xF033};
    static constexpr uint16_t STORE_V0_UPTO_VX{0xF055};
    static constexpr uint16_t FILL_V0_UPTO_VX{0xF065};


    // Define the static member fontset
    static constexpr uint8_t fontset[FONT_SIZE]
    {
        // 0
        0xF0, 0x90, 0x90, 0x90, 0xF0,

        // 1
        0x20, 0x60, 0x20, 0x20, 0x70,

        // 2
        0xF0, 0x10, 0xF0, 0x80, 0xF0,

        // 3
        0xF0, 0x10, 0xF0, 0x10, 0xF0,

        // 4
        0x90, 0x90, 0xF0, 0x10, 0x10,

        // 5
        0xF0, 0x80, 0xF0, 0x10, 0xF0,

        // 6
        0xF0, 0x80, 0xF0, 0x90, 0xF0,

        // 7
        0xF0, 0x10, 0x20, 0x40, 0x40,

        // 8
        0xF0, 0x90, 0xF0, 0x90, 0xF0,

        // 9
        0xF0, 0x90, 0xF0, 0x10, 0xF0,

        // A
        0xF0, 0x90, 0xF0, 0x90, 0x90,

        // B
        0xE0, 0x90, 0xE0, 0x90, 0xE0,

        // C
        0xF0, 0x80, 0x80, 0x80, 0xF0,

        // D
        0xE0, 0x90, 0x90, 0x90, 0xE0,

        // E
        0xF0, 0x80, 0xF0, 0x80, 0xF0,

        // F
        0xF0, 0x80, 0xF0, 0x80, 0x80
    };

    // code to string map
    static inline const std::unordered_map<uint16_t, std::string_view> opcode_map{
    {CALL, "CALL"},
    {CLR_DISPLAY, "CLR_DISPLAY"},
    {RETURN_FROM_ROUTINE, "RETURN_FROM_ROUTINE"},
    {JUMP_TO_NNN, "JUMP_TO_NNN"},
    {CALL_ROUTINE, "CALL_ROUTINE"},
    {IF_VX_EQ_NN, "IF_VX_EQ_NN"},
    {IF_VX_NOT_NN, "IF_VX_NOT_NN"},
    {IF_VX_EQ_VY, "IF_VX_EQ_VY"},
    {SET_VX_TO_NN, "SET_VX_TO_NN"},
    {ADD_NN_TO_VX, "ADD_NN_TO_VX"},
    {SET_VX_TO_VY, "SET_VX_TO_VY"},
    {SET_VX_TO_VX_OR_VY, "SET_VX_TO_VX_OR_VY"},
    {SET_VX_TO_VX_AND_VY, "SET_VX_TO_VX_AND_VY"},
    {SET_VX_TO_VX_XOR_VX, "SET_VX_TO_VX_XOR_VX"},
    {ADD_VY_TO_VX, "ADD_VY_TO_VX"},
    {SUB_VY_FROM_VX, "SUB_VY_FROM_VX"},
    {SHIFT_VX_RIGHT, "SHIFT_VX_RIGHT"},
    {SUB_VX_FROM_VY, "SUB_VX_FROM_VY"},
    {SHIFT_VX_LEFT, "SHIFT_VX_LEFT"},
    {IF_VX_NOTEQ_VY, "IF_VX_NOTEQ_VY"},
    {SET_I_TO_NNN, "SET_I_TO_NNN"},
    {JUMP_TO_NNN_PLUS_V0, "JUMP_TO_NNN_PLUS_V0"},
    {SET_VX_RANDOM, "SET_VX_RANDOM"},
    {DRAW, "DRAW"},
    {IF_KEY_EQ_VX, "IF_KEY_EQ_VX"},
    {IF_KEY_NOT_EQ_VX, "IF_KEY_NOT_EQ_VX"},
    {SET_VX_TO_DELAY, "SET_VX_TO_DELAY"},
    {SET_VX_TO_KEY, "SET_VX_TO_KEY"},
    {SET_DELAY_TO_VX, "SET_DELAY_TO_VX"},
    {SET_SOUND_TO_VX, "SET_SOUND_TO_VX"},
    {ADD_VX_TO_I, "ADD_VX_TO_I"},
    {SET_I_TO_SPRITE_ADDR, "SET_I_TO_SPRITE_ADDR"},
    {STORE_BCD, "STORE_BCD"},
    {STORE_V0_UPTO_VX, "STORE_V0_UPTO_VX"},
    {FILL_V0_UPTO_VX, "FILL_V0_UPTO_VX"}
};

    // Memory and registers
    uint8_t memory[MEMORY_SIZE]{};
    uint8_t V[REGISTER_COUNT]{};
    uint16_t I{}; // size of I is 12bit
    uint16_t pc{0x200};
    bool keypad[KEY_PAD_SIZE]{}; // 0 to F
    uint16_t stack[STACK_SIZE]{};
    uint8_t sp{};

    // Timers
    uint8_t delayTimer{0};
    uint8_t soundTimer{0};
    uint64_t lastTime{0};

    // Random number tools
    std::mt19937 gen;
    std::uniform_int_distribution<std::mt19937::result_type> distrib;

    //display
    DisplayGrid display{};

    void clrmem();
    void clrreg();
    void clrstack();
    void loadfonts();
    uint16_t fetchOpcode();
    void decode(uint16_t);

    // returns the number of register X
    uint8_t getX(uint16_t);

    // returns the number of register Y
    uint8_t getY(uint16_t);

    // return the 8 LSB
    uint8_t getNN(uint16_t);

    //returns the 12 LSB
    uint16_t getNNN(uint16_t);


    void printCode(uint16_t);
    void printHex(uint16_t);

    // prints the value of the register
    void printV(uint16_t);

    // operations
    void setVxToNN(uint16_t);
    void addNNtoVx(uint16_t);
    void setVxToVy(uint16_t);
    void setVxToVxOrVy(uint16_t);
    void setVxToVxAndVy(uint16_t);
    void setVxToVxXorVy(uint16_t);
    void AddVyToVx(uint16_t);
    void subVyFromVx(uint16_t);
    void shiftVxRight(uint16_t);
    void subVxFromVy(uint16_t);
    void shiftVxLeft(uint16_t);
    void setIToNNN(uint16_t);
    void setVxRandom(uint16_t);
    void addVxToI(uint16_t);
    void jumpToNNNPlusV0(uint16_t);
    void jumpToNNN(uint16_t);
    void callRoutine(uint16_t);
    void ifVxEqNN(uint16_t);
    void ifVxNotNN(uint16_t);
    void ifVxEqVy(uint16_t);
    void ifVxNoteqVy(uint16_t);
    void returnFromRoutine();
    void fillV0UpToVx(uint16_t);
    void storeV0UpToVx(uint16_t);
    void storeBCD(uint16_t);
    void setIToSpriteAddr(uint16_t);
    void setSoundToVx(uint16_t);
    void setDelayToVx(uint16_t);
    void setVxToDelay(uint16_t);
    void setVxToKey(uint16_t);
    void ifKeyNotEqVx(uint16_t);
    void ifKeyEqVx(uint16_t);
    void clrDisplay();
    void draw(uint16_t);
};
