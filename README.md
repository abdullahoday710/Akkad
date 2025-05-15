# Akkad

**Akkad** is a 2D game engine developed as a personal learning project focused on hardware-accelerated rendering and general system architecture and design.

This engine is built entirely from scratch—without using any external windowing libraries (such as GLFW or SDL) and without relying on rendering libraries like bgfx.

## Features

- **Custom Windowing System**  
  Implements its own native window and input handling without third-party dependencies.

- **Rendering Abstraction Layer**  
  Provides a unified API for both **OpenGL** and **OpenGL ES**, supporting a range of platforms.

- **Cross-Platform Support**  
  - **Windows (Desktop)** — Runs as a native desktop application.  
  - **Web (Browser)** — Compiles to **WebAssembly** via [Emscripten](https://emscripten.org/), allowing the engine to run directly in modern browsers.
