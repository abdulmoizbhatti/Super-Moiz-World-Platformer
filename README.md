Welcome to Super Moiz World. A 2D platformer inspired by classics such as Mario and Sonic.

## Demo
[![Watch on YouTube](https://img.shields.io/badge/Watch%20on-YouTube-red?style=for-the-badge&logo=youtube)](https://www.youtube.com/watch?v=Y8IaefpZG-0)

---

## Steps:

These steps are for macOS. For Windows/Linux you may need to download dependencies differently, such as getting a C++17 compatible compiler, and also adjust the build command accordingly.

1. Clone repo
2. Install homebrew if you don't already have it (macOS).
3. With homebrew/your method of installation, install pugixml for XML parsing and SFML for graphics.
4. Build command: clang++ -std=c++17 -I/opt/homebrew/include -I/opt/homebrew/include/SFML -I./tmxlite/tmxlite/include -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system -lpugixml src/*.cpp tmxlite/tmxlite/src/*.cpp tmxlite/tmxlite/src/miniz.c -o game
5. ./game

---

## Special Thanks:

1. craftpix.net for supplying the open source background (https://craftpix.net/freebies/free-horizontal-2d-game-backgrounds/) and character sprites (https://craftpix.net/freebies/free-shinobi-sprites-pixel-art/)
2. Creators of tmxlite to help parse Tiled tsx files into my C++ code. https://github.com/fallahn/tmxlite
