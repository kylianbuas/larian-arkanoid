# Breakout

A small Arkanoid/Breakout clone written in C++20 with SDL3 with a level editor built with ImGui.

The game and the level editor share the same engine library (`arkanoid_lib`), so levels created in the editor can be dropped straight into the game.

## Features

- Brick-breaking gameplay with paddle-angle-based ball bouncing and a combo counter that raises the pitch of hit sounds
- Seven bundled levels played in a loop
- Three lives, sounds, animations and UI
- Data-driven assets: textures, sounds, font, and levels are loaded from `assets/` at runtime
- Level editor with a 12×18 grid, brick palette, and native open/save dialogs

## Building

The easiest way is to run the provided script from a normal Command Prompt:

```bat
generate.bat
```

This configures a Visual Studio 2022 x64 solution in `build/`, copies files from `setup/` into it, and leaves you with `build/arkanoid.sln`.

To configure and build manually:

```bat
cmake -G "Visual Studio 17 2022" -A x64 -S . -B build
cmake --build build --config Debug
```

Build either configuration (`Debug` or `Release`). The build copies the SDL DLLs and the `assets/` folder next to each executable, so the programs can be run directly from the output directory:

## Third-party libraries

- [SDL3](https://github.com/libsdl-org/SDL)
- [SDL3_image](https://github.com/libsdl-org/SDL_image)
- [SDL3_ttf](https://github.com/libsdl-org/SDL_ttf)
- [Dear ImGui](https://github.com/ocornut/imgui)