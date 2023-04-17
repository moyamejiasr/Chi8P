# Chip8 Emulator in C++

This is a Chip8 emulator written in C++ with SDL2 for graphics and input. This emulator allows you to run Chip8 ROMs on your computer.

⚠ *This project is still in development and may lack some instructions or functionalities.*

## How to Use

1. Clone the repository
2. Run the install script `./install.sh` (On Debian-like systems)
3. Compile the program with `make`
4. Run the executable file with `./chip8`

## Key Mapping

The original Chip8 keypad had 16 keys arranged in a 4x4 matrix:

| 1 | 2 | 3 | C |
|---|---|---|---|
| 4 | 5 | 6 | D |
| 7 | 8 | 9 | E |
| A | 0 | B | F |

The emulator maps these keys to your computer's keyboard as follows:

| Chip8 Key | Keyboard Key |
|-----------|--------------|
| 1         | 1            |
| 2         | 2            |
| 3         | 3            |
| 4         | 4            |
| 5         | 5            |
| 6         | 6            |
| 7         | 7            |
| 8         | 8            |
| 9         | 9            |
| A         | A            |
| B         | B            |
| C         | C            |
| D         | D            |
| E         | E            |
| F         | F            |
| 0         | 0            |

## References

- [Chip-8 Design Specification](http://www.cs.columbia.edu/~sedwards/classes/2016/4840-spring/designs/Chip8.pdf)
- [Chip8 Wikipedia Page](https://en.wikipedia.org/wiki/CHIP-8)
- [SDL2 Library](https://www.libsdl.org/)
