# Conway's Game of Life

A simple interactive implementation of **Conway's Game of Life** using **SDL3**. The program displays a grid where you can toggle cells with the mouse and step through generations with the keyboard.

## Rules

  * Conway's rules:
    1. Live cell with 2 or 3 neighbors survives.
    2. Dead cell with exactly 3 neighbors becomes alive.
    3. All other cells die or remain dead.

## Requirements

* **C compiler** (GCC, Clang, or MSVC)
* **SDL3** library installed

## Installation / Build

1. Make sure SDL3 is installed on your system.

   **Linux (Debian/Ubuntu):**

   ```bash
   sudo apt install libsdl3-dev
   ```

   **Windows:**

   * Download SDL3 development libraries from [SDL website](https://libsdl.org/download-3.0.php)
   * Set include and lib paths in your compiler or IDE.

2. Compile the program:

   **Linux/macOS:**

   ```bash
   gcc main.c -o gol -lSDL3
   ```

   **Windows (MinGW):**

   ```bash
   gcc main.c -o gol.exe -lSDL3main -lSDL3
   ```

3. Run the program:

   ```bash
   ./gol
   ```

   On Windows:

   ```bash
   gol.exe
   ```

## Usage

1. When the program starts, it will prompt you to enter grid dimensions:

   ```
   Enter width and height (default 40 24):
   ```

   * Press **Enter** to use defaults (`40x24`) or type your desired size.

2. The window opens with an empty grid.

3. **Mouse controls:**

   * Click a cell to toggle it alive/dead.

4. **Keyboard controls:**

   * `Space` — advance one generation.
   * `Close window` or `Ctrl+C` — exit the program.


