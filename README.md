<img width="720" height="720" alt="Adobe Express - 2026-06-03 04-28-15 (1)" src="https://github.com/user-attachments/assets/1a9bfe73-565c-4dae-a7e5-2630792d3c77" />


Tic-Tac-Toe (SFML & C++)
A dynamic, state-driven Tic-Tac-Toe game engine built in C++ using the SFML framework. It features an expandable grid system, polymorphism-based UI menus, and safe memory management tracking.

🚀 Features
Custom Grid Scaling: Adjust board sizing and win-sequence lengths dynamically in the settings menu.

Polymorphic UI System: Seamlessly switches screens (MainMenu, PlaySettings, GameOver) inheriting from a base MenuScreen class.

Memory Safety: Prevents dangling pointer crashes by populating smart pointer containers (std::unique_ptr) before caching raw tracking references.

Reactive Layout Rebuilds: The Game Over screen dynamically clears and updates text strings and player colors the exact frame a match ends.

Audio System: Fully integrated background music loop and discrete sound effects for piece placement and game-over triggers.

🎮 Controls
Left Click: Select menu options and place grid pieces.

Escape (ESC): Instantly drop an active match and exit back to the Main Menu.
