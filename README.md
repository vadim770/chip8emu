# CHIP-8 Emulator

## Getting Started

### 1. Prerequisites

To build and run the emulator, you will need a C++ compiler supporting **C++20** and the **SDL3** development library.

#### Windows (via MSYS2 - Recommended)
1. Install [MSYS2](https://www.msys2.org/).
2. Open the **MSYS2 UCRT64** terminal and install the toolchain and SDL3 packages:
   ```bash
   pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-sdl3
   ```

#### Linux (Debian/Ubuntu)
Install GCC/Clang and the SDL3 development packages:
```bash
sudo apt install build-essential libsdl3-dev
```

#### macOS (via Homebrew)
Install CMake (optional) and the SDL3 package:
```bash
brew install sdl3
```

---

### 2. Building the Project

Run the following compilation command inside the repository root directory:

```bash
g++ -std=c++20 -O3 -DSDL_MAIN_HANDLED src/*.cpp src/imgui/*.cpp -Isrc/imgui -o src/chip8emu.exe -lSDL3
```

---

### 3. Running the Emulator

Run the generated executable from the project root:

```bash
./src/chip8emu.exe
```

Use the **File -> Open ROM...** menu to select and play any game from the bundled `roms/` folder or any other `.ch8` binary.

---

## Keyboard Controls

The original CHIP-8 uses a **16-key hex keypad (0-F)**. This emulator maps them to standard PC keyboard keys ergonomically:

| CHIP-8 Keypad | PC QWERTY Mapping |
| :---: | :---: |
| **`1` `2` `3` `C`** | **`1` `2` `3` `4`** |
| **`4` `5` `6` `D`** | **`Q` `W` `E` `R`** |
| **`7` `8` `9` `E`** | **`A` `S` `D` `F`** |
| **`A` `0` `B` `F`** | **`Z` `X` `C` `V`** |

---

## License

This project is open-source and available under the MIT License. Feel free to use, modify, and distribute as you wish!
