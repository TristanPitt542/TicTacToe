# Dynamic State-Driven Tic-Tac-Toe Engine (C++ & SFML)

A dynamic, state-driven 2D game engine and application built in modern C++ using SFML 3.1 and CMake. Designed with a clean architectural separation of concerns, the engine features an expandable grid system, polymorphism-based screen management, safe smart pointer ownership tracking, and integrated audio response loops.

## 🚀 Key Architectural Features

* **Dynamic Grid Scaling:** Runtime modification of board dimensions (N×N) and win-sequence lengths configurable directly via application settings.
* **Polymorphic UI Framework:** State-driven screen stack (`MainMenu`, `PlaySettings`, `GameOver`) inheriting from an abstract `MenuScreen` base class for zero-leak transition management.
* **Strict Ownership & Memory Hygiene:** Clean lifetime tracking utilizing `std::unique_ptr` ownership containers before caching raw observation pointers, eliminating dangling reference vectors.
* **Reactive Layout Engine:** Immediate single-frame layout recalculation and UI string/color state rebuilds upon game loop state changes (e.g., match termination).
* **Audio & Spatial FX Integration:** Asynchronous background music loop playback and discrete low-latency SFX triggers for piece placement and game-over states.

## 🛠️ Tech Stack

* **Language:** C++17 / Modern C++
* **Graphics & Inputs:** SFML 3.1 (Simple and Fast Multimedia Library)
* **Build System:** CMake
* **Target Platforms:** Windows / Linux

## 🎮 Controls & Interface

* **Mouse Left-Click:** Navigate polymorphic menus, tweak settings toggles, and place board grid pieces.
* **Escape (ESC):** Instantly drop active match state and transition gracefully back to the `MainMenu` state.

## 🔨 Build & Run Instructions

Using CMD:
1. git clone https://github.com/TristanPitt542/TicTacToe.git
2. cd TicTacToe
3. cmake -B build
4. cmake --build build --config Release
5. .\build\Release\TicTacToe.exe

---

Using Visual Studio / VS Code:
1. Open Project in VS Code
2. Make sure CMake Tools & C/C++ is installed
3. CTRL + SHIFT + P
4. CMake: configure
5. CMake: Build
6. Run
