#pragma once
#include <memory>
#include "Player.hpp"
#include "Platform.hpp"
#include "Level.hpp"
#include "Assets.hpp"
#include <SFML/Graphics.hpp>

class Game {
public:
    Game();
    void run();
private:
    sf::RenderWindow window;
    std::unique_ptr<Player> player;
    std::vector<Platform> platforms;
    std::unique_ptr<Level> level;
    std::unique_ptr<Assets> assets;
    // Background
    sf::Texture backgroundTexture;
    std::unique_ptr<sf::Sprite> backgroundSprite;
    void processEvents();
    void update(float dt);
    void render();
    void handleCollisions(float dt);
    void resetPlayer();
}; 