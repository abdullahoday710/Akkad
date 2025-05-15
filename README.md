# Akkad

**Akkad** is a 2D game engine developed as a personal learning project focused on hardware-accelerated rendering and general system architecture and design.

The engine is built entirely from scratch—without using any external windowing libraries (such as GLFW or SDL) and without relying on rendering libraries like bgfx.

**Note that I have stopped working on this project from a long time ago, And it is no longer maintained**

## Features

- **Custom Windowing System**  
  Implements its own native window and input handling without third-party dependencies.

- **Rendering Abstraction Layer**  
  Provides a unified API for both **OpenGL** and **OpenGL ES**, supporting a range of platforms.

- **Cross-Platform Support**  
  - **Windows (Desktop)** — Runs as a native desktop application.  
  - **Web (Browser)** — Compiles to **WebAssembly** via [Emscripten](https://emscripten.org/), allowing the engine to run directly in modern browsers.

## File Structure

- `Akkad/` — The engine core, built as a static library.
- `Editor/` — The editor application for creating levels and scripting gameplay.
- `Runtime/` — The runtime executable, intended to be the final distributable version of the game.


## Akkad static library in details :

- `src/Akkad/Graphics` — The graphics abstraction layer sub system
- `src/Akkad/Platforms` — Platform implementations (currently only desktop windows and emscripten)
- `src/Akkad/ECS` — Entity component system
- `src/Akkad/Asset` — Asset management sub system
