#pragma once
#include <memory>
#include "Player.hpp"
#include "Assets.hpp"
#include <SFML/Graphics.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>

class Game {
public:
    Game();
    void run();
private:
    sf::RenderWindow window;
    std::unique_ptr<Player> player;
    std::unique_ptr<Assets> assets;
    // Background
    sf::Texture backgroundTexture;
    std::unique_ptr<sf::Sprite> backgroundSprite;
    // Tiled map
    tmx::Map tiledMap;
    std::vector<sf::Sprite> tileSprites;
    std::vector<std::unique_ptr<sf::Texture>> tilesetTextures;
    void processEvents();
    void update(float dt);
    void render();
    void handleCollisions(float dt);
    void resetPlayer();
}; 