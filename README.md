# SKKU Tetris

A classic Tetris game written in **C** with **SDL2**, built from scratch for the *Basis and Practice in Programming* course (Fall 2025) at Sungkyunkwan University.

<!-- TODO: Add a screenshot or GIF here! e.g. -->
<!-- ![Gameplay](screenshots/gameplay.gif) -->

## Features

- **All 7 tetrominoes** (I, J, L, O, S, T, Z), each with 4 rotation states, defined in a 4D shape array
- **Full movement & collision system** — left/right movement, rotation, and soft drop, all with boundary and locked-block collision checks
- **Line clearing** with rows shifting down correctly, including multi-line clears
- **Scoring system** — 100 / 300 / 500 / 800 points for 1 / 2 / 3 / 4 (Tetris!) simultaneous line clears
- **Progressive difficulty** — fall speed increases as your score passes level thresholds
- **Persistent leaderboard** — top 10 scores saved to a binary file (`leaderboard.dat`) and loaded on startup
- **Color-coded pieces** — each tetromino type keeps its own color after locking
- Runs at a capped **60 FPS** with VSync-enabled hardware-accelerated rendering

## Controls

| Key | Action |
|-----|--------|
| ← / → | Move piece left / right |
| ↑ | Rotate piece |
| ↓ | Soft drop |
| ESC | Quit |

## Building and Running

### Requirements
- A C compiler (gcc / clang)
- [SDL2](https://www.libsdl.org/)

### macOS
```bash
brew install sdl2
gcc tetris.c -o tetris $(sdl2-config --cflags --libs)
./tetris
```

### Linux (Debian/Ubuntu)
```bash
sudo apt install libsdl2-dev
gcc tetris.c -o tetris $(sdl2-config --cflags --libs)
./tetris
```

## How It Works

- The board is a `20 × 10` grid (`int board[ROWS][COLUMNS]`) where `0` = empty and values `2–8` = locked blocks of each piece type (which is how each locked piece remembers its color).
- The falling piece is described by four state variables: position (`x`, `y`), type (`0–6`), and rotation (`0–3`), which index into the `shapes[7][4][4][4]` lookup table.
- Every frame, the game loop polls input, applies gravity on a timer (`fallDelay`, which shrinks as you level up), locks pieces that can no longer fall, scans for full rows, updates the score, and redraws the board.
- On game over, the final score is inserted into the leaderboard, sorted with `qsort`, and written back to disk with `fwrite`.

## Acknowledgments

Built for the *Basis and Practice in Programming* course at SKKU, taught by Prof. Sun Jae Lee, starting from the course's SDL2 skeleton (window/renderer setup and drawing helpers). All game logic — piece definitions, movement, rotation, collision, line clearing, scoring, levels, and the leaderboard — implemented by me.

## Author

**Emil Ganbarli** — Software Engineering @ SKKU
GitHub: [emilimperator](https://github.com/emilimperator)
