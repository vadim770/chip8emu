#include "../include/Chip8.hpp"


void Chip8::decode(uint16_t opcode){
    uint16_t ops1 = opcode & 0xF000;
    uint16_t ops2 = opcode & 0xF00F;
    uint16_t ops3 = opcode & 0xF0FF;


    switch(ops1){
        case JUMP_TO_NNN:
            // printCode(ops1);
            jumpToNNN(opcode);
            return;
        case CALL_ROUTINE:
            // printCode(ops1);
            callRoutine(opcode);
            return;
        case IF_VX_EQ_NN:
            // printCode(ops1);
            ifVxEqNN(opcode);
            return;
        case IF_VX_NOT_NN:
            // printCode(ops1);
            ifVxNotNN(opcode);
            return;
        case IF_VX_EQ_VY:
            // printCode(ops1);
            ifVxEqVy(opcode);
            return;
        case SET_VX_TO_NN:
            // printCode(ops1);
            setVxToNN(opcode);
            // printV((opcode & 0x0F00) >> 8);
            return;
        case ADD_NN_TO_VX:
            // printCode(ops1);
            addNNtoVx(opcode);
            // printV((opcode & 0x0F00) >> 8);
            return;
        
        case IF_VX_NOTEQ_VY:
            // printCode(ops1);
            ifVxNoteqVy(opcode);
            return;
        case SET_I_TO_NNN:
            // printCode(ops1);
            setIToNNN(opcode);
            return;
        case JUMP_TO_NNN_PLUS_V0:
            // printCode(ops1);
            jumpToNNNPlusV0(opcode);
            return;
        case SET_VX_RANDOM:
            // printCode(ops1);
            setVxRandom(opcode);
            return;
        case DRAW:
            // printCode(ops1);
            draw(opcode);
            return;
    }

    switch(ops2){
        case SET_VX_TO_VY:
            // printCode(ops2);
            setVxToVy(opcode);
            // printV((opcode & 0x0F00) >> 8);
            return;
        case SET_VX_TO_VX_OR_VY:
            // printCode(ops2);
            setVxToVxOrVy(opcode);
            // printV((opcode & 0x0F00) >> 8);
            return;
        case SET_VX_TO_VX_AND_VY:
            // printCode(ops2);
            setVxToVxAndVy(opcode);
            return;
        case SET_VX_TO_VX_XOR_VX:
            // printCode(ops2);
            setVxToVxXorVy(opcode);
            return;
        case ADD_VY_TO_VX:
            // printCode(ops2);
            AddVyToVx(opcode);
            return;
        case SUB_VY_FROM_VX:
            // printCode(ops2);
            subVyFromVx(opcode);
            return;
        case SHIFT_VX_RIGHT:
            // printCode(ops2);
            shiftVxRight(opcode);
            return;
        case SUB_VX_FROM_VY:
            // printCode(ops2);
            subVxFromVy(opcode);
            return;
        case SHIFT_VX_LEFT:
            // printCode(ops2);
            shiftVxLeft(opcode);
            return;
    }

    switch(ops3){
        case IF_KEY_EQ_VX:
            // printCode(ops3);
            ifKeyEqVx(opcode);
            return;
        case IF_KEY_NOT_EQ_VX:
            // printCode(ops3);
            ifKeyNotEqVx(opcode);
            return;

        case SET_VX_TO_DELAY:
            // printCode(ops3);
            setVxToDelay(opcode);
            return;
        case SET_VX_TO_KEY:
            // printCode(ops3);
            setVxToKey(opcode);
            return;
        case SET_DELAY_TO_VX:
            // printCode(ops3);
            setDelayToVx(opcode);
            return;
        case SET_SOUND_TO_VX:
            // printCode(ops3);
            setSoundToVx(opcode);
            return;
        case ADD_VX_TO_I:
            // printCode(ops3);
            addVxToI(opcode);
            return;
        case SET_I_TO_SPRITE_ADDR:
            // printCode(ops3);
            setIToSpriteAddr(opcode);
            return;
        case STORE_BCD:
            // printCode(ops3);
            storeBCD(opcode);
            return;
        case STORE_V0_UPTO_VX:
            // printCode(ops3);
            storeV0UpToVx(opcode);
            return;
        case FILL_V0_UPTO_VX:
            // printCode(ops3);
            fillV0UpToVx(opcode);
            return;
    }

    if(opcode == 0x00EE){
        // printCode(0x00EE);
        returnFromRoutine();
        return;
    }

    if(opcode == 0x00E0){
        // printCode(0x00E0);
        clrDisplay();
        return;
    }

    // printCode(0x0000);

    
}

void Chip8::draw(uint16_t opcode){
    uint8_t x = getX(opcode);
    uint8_t y = getY(opcode);
    uint8_t n = static_cast<uint8_t>(opcode & 0x000F);
    uint8_t offset{0};

    V[0xF] = 0;
    for(uint8_t i=V[y], counter=0; counter < n; i++, counter++){
            uint8_t mask = 0x80;
            uint8_t bits = memory[I+offset];
        for(uint8_t j=V[x], widthCounter = 0; widthCounter < PIXEL_WIDTH; j++, mask >>= 1, widthCounter++){
            if(!(mask & bits)){
                continue;
            }

            if(!display[i % 32][j % 64]){
                display[i % 32][j % 64] = true;
            }
            else{
                V[0xF] = 1;
                display[i % 32][j % 64] = false;
            }
        }
        offset++;
    }
}

void Chip8::decDelayTimer(){
    if(delayTimer > 0)
        delayTimer--;
}

void Chip8::decSoundTimer(){
    if(soundTimer > 0)
        soundTimer--;
}

void Chip8::setKey(uint8_t key){
    keypad[key] = true;
}

void Chip8::unsetKey(uint8_t key){
    keypad[key] = false;
}

const Chip8::DisplayGrid& Chip8::getDisplay() const {
    return display;
}

const uint8_t& Chip8::getSoundTimer() const{
    return soundTimer;
}

void Chip8::ifKeyEqVx(uint16_t opcode){
    uint8_t x = getX(opcode);

    if(keypad[V[x] & 0xF]){
        pc += 2;
    }
}

void Chip8::ifKeyNotEqVx(uint16_t opcode){
    uint8_t x = getX(opcode);
    
    if(!keypad[V[x] & 0xF]){
        pc += 2;
    }
}

void Chip8::setVxToKey(uint16_t opcode){
    uint8_t x = getX(opcode);
    for(uint8_t i=0; i<KEY_PAD_SIZE; i++){
        if(keypad[i]){
            V[x] = i;
            return;
        }
    }
    pc -= 2;

}

void Chip8::setVxToDelay(uint16_t opcode){
    uint8_t x = getX(opcode);
    V[x] = delayTimer;
}

void Chip8::setDelayToVx(uint16_t opcode){
    uint8_t x = getX(opcode);
    delayTimer = V[x];
}

void Chip8::setSoundToVx(uint16_t opcode){
    uint8_t x = getX(opcode);
    soundTimer = V[x];
}

void Chip8::setIToSpriteAddr(uint16_t opcode){
    uint8_t x = getX(opcode);
    uint8_t sprite = V[x] & 0xF;
    I = sprite * 5;
}

void Chip8::storeBCD(uint16_t opcode){
    uint8_t x = getX(opcode);
    uint16_t value = V[x];

    for(int8_t i = 2; i >= 0 ; i--){
        memory[I+i] = uint8_t(value % 10);
        value = value / 10;
    }
}

void Chip8::storeV0UpToVx(uint16_t opcode){
    uint16_t counter = (opcode & 0x0F00) >> 8;
    for(uint16_t x=0; x <= counter; x++){
        memory[I+x] = uint8_t(V[x]);
    }
}

void Chip8::fillV0UpToVx(uint16_t opcode){
    uint16_t counter = (opcode & 0x0F00) >> 8;
    for(uint16_t x=0; x <= counter; x++){
        V[x] = uint8_t(memory[I+x]);
    }
}

void Chip8::returnFromRoutine(){
    pc = stack[--sp];
    pc -= 2;
}

void Chip8::ifVxNoteqVy(uint16_t opcode){
    uint8_t x = getX(opcode);
    uint8_t y = getY(opcode);
    if(V[x] != V[y])
        pc += 2;
}

void Chip8::ifVxEqVy(uint16_t opcode){
    uint8_t x = getX(opcode);
    uint8_t y = getY(opcode);
    if(V[x] == V[y])
        pc += 2;
}

void Chip8::ifVxNotNN(uint16_t opcode){
    uint8_t x = getX(opcode);
    uint8_t nn = getNN(opcode);
    if(V[x] != nn)
        pc += 2;
}

void Chip8::ifVxEqNN(uint16_t opcode){
    uint8_t x = getX(opcode);
    uint8_t nn = getNN(opcode);
    if(V[x] == nn)
        pc += 2;
    
}

void Chip8::callRoutine(uint16_t opcode){
    stack[sp++] = pc + 2;
    pc = getNNN(opcode);
    pc -= 2;
}

void Chip8::jumpToNNN(uint16_t opcode){
    pc = getNNN(opcode);
    pc -= 2;
}

void Chip8::jumpToNNNPlusV0(uint16_t opcode){
    pc = (getNNN(opcode) + V[0] - 2) & 0x0FFF;
}

void Chip8::setVxToNN(uint16_t opcode){
    uint8_t x = getX(opcode);
    uint8_t nn = getNN(opcode);
    V[x] = (nn);
}

// void Chip8::printCode(uint16_t code){
//         std::cout << opcode_map.at(code) << '\n';
// }

void Chip8::initialize() {
    clrmem();
    clrreg();
    clrstack();
    loadfonts();
    clrDisplay();
    I = 0;
    pc = 0x200;
    sp = 0;
    delayTimer = 0;
    soundTimer = 0;
    std::random_device rd;
    gen.seed(rd());
    distrib = std::uniform_int_distribution<std::mt19937::result_type>(0, 255);
}

void Chip8::addVxToI(uint16_t opcode){
    uint8_t x = getX(opcode);
    I = (I + V[x]) & 0x0FFF;
}

void Chip8::setVxRandom(uint16_t opcode){
    uint8_t x = getX(opcode);
    uint8_t nn = getNN(opcode);
    V[x] = uint8_t(nn & distrib(gen));
}

uint16_t Chip8::getNNN(uint16_t opcode){
    return opcode & 0x0FFF;
}

void Chip8::setIToNNN(uint16_t opcode){
    I = getNNN(opcode);
}

void Chip8::shiftVxLeft(uint16_t opcode){
    uint8_t x = getX(opcode);
    V[0xF] = V[x] & 0x80;
    V[x] = V[x] << 0x1;
}

void Chip8::subVxFromVy(uint16_t opcode){
    uint8_t x = getX(opcode);
    uint8_t y = getY(opcode);

    if(V[y] >= V[x])
        V[0xF] = 1;
    else
        V[0xF] = 0;

    V[x] = V[y] - V[x];
}

void Chip8::shiftVxRight(uint16_t opcode){
    uint8_t x = getX(opcode);
    V[0xF] = V[x] & 0x1;
    V[x] = V[x] >> 0x1;
}

uint8_t Chip8::getX(uint16_t opcode){
    return (opcode & 0x0F00) >> 8;
}

uint8_t Chip8::getY(uint16_t opcode){
    return (opcode & 0x00F0) >> 4;
}

void Chip8::subVyFromVx(uint16_t opcode){
    uint8_t x = getX(opcode);
    uint8_t y = getY(opcode);
    if(V[x] >= V[y])
        V[0xF] = 1;
    else
        V[0xF] = 0;

    V[x] -= V[y];
}

uint8_t Chip8::getNN(uint16_t opcode){
    return uint8_t(opcode & 0x00FF);
}

void Chip8::AddVyToVx(uint16_t opcode){
    uint8_t x = getX(opcode);
    uint8_t y = getY(opcode);
    uint16_t prevVal = V[x];
    V[x] += V[y];
    if(prevVal > V[x])
        V[0xF] = 1;
    else
        V[0xF] = 0;
}

void Chip8::setVxToVxXorVy(uint16_t opcode){
    uint8_t x = getX(opcode);
    uint8_t y = getY(opcode);
    V[x] = V[x] ^ V[y];
}

void Chip8::setVxToVxAndVy(uint16_t opcode){
    uint8_t x = getX(opcode);
    uint8_t y = getY(opcode);
    V[x] = V[x] & V[y];
}

void Chip8::setVxToVxOrVy(uint16_t opcode){
    uint8_t x = getX(opcode);
    uint8_t y = getY(opcode);
    V[x] = V[x] | V[y];
}

void Chip8::addNNtoVx(uint16_t opcode){
    uint8_t x = getX(opcode);
    uint8_t nn = getNN(opcode);
    V[x] += (nn);
}

void Chip8::setVxToVy(uint16_t opcode){
    uint8_t x = getX(opcode);
    uint8_t y = getY(opcode);
    V[x] = V[y];
}

// void Chip8::printHex(uint16_t num){
//         std::cout << "0x"
//             << std::hex << std::uppercase
//             << std::setw(4) << std::setfill('0') 
//             << std::setw(4) << num
//             << std::dec << '\n';
// }

void Chip8::cycle(){
    
    uint16_t opcode = fetchOpcode();
    // printHex(opcode);
    if(opcode == 0)
        return;

    decode(opcode);

    pc += 2;
    
}

void Chip8::clrDisplay(){
    for(int i=0; i<DISPLAY_HEIGHT; i++){
        for(int j=0; j<DISPLAY_WIDTH; j++){
            display[i][j] = false;
        }
    }
}

uint16_t Chip8::fetchOpcode(){
    return (memory[pc] << 8) | memory[pc + 1];
}

long long int Chip8::loadROM(std::string_view path) {
    std::ifstream file(std::filesystem::path(path), std::ios::binary);
    if (!file) {
        std::cerr << "Could not open the file\n";
        return -1;
    }

    file.seekg(0, std::ios::end);
    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    if (fileSize > MAX_FILE_SIZE) {
        std::cerr << "The file exceeds the 3584-byte limit\n";
        return -1;
    }

    file.read(reinterpret_cast<char*>(&memory[0x200]), fileSize);
    loadedRom = true;
    filepath = path;
    return fileSize;
}

void Chip8::clrmem() {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        memory[i] = 0;
    }
}

void Chip8::clrreg() {
    for (int i = 0; i < REGISTER_COUNT; i++) {
        V[i] = 0;
    }
}

void Chip8::clrstack() {
    for (int i = 0; i < STACK_SIZE; i++) {
        stack[i] = 0;
    }
}

void Chip8::loadfonts() {
    for (int i = 0; i < FONT_SIZE; i++) {
        memory[i] = fontset[i];
    }
}

// void Chip8::printV(uint16_t regnum){
//     std::cout << "Register V" << regnum << ": ";
//     printHex(V[regnum]);
// }

bool Chip8::isLoaded(){
    if(loadedRom)
        return true;
    return false;
}
