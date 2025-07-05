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
    enum class UIState { StartScreen, WorldSelect, Playing, LevelComplete };
    UIState uiState = UIState::StartScreen;
    // UI assets
    sf::Texture startScreenBgTexture;
    std::unique_ptr<sf::Sprite> startScreenBgSprite;
    sf::Font uiFont;
    std::unique_ptr<sf::RectangleShape> playButton;
    std::unique_ptr<sf::Text> titleText;
    std::unique_ptr<sf::Text> playButtonText;
    std::unique_ptr<sf::RectangleShape> worldButton;
    std::unique_ptr<sf::Text> worldButtonText;
    std::unique_ptr<sf::RectangleShape> backButton;
    std::unique_ptr<sf::Text> backButtonText;
    std::unique_ptr<sf::RectangleShape> levelCompleteButton;
    std::unique_ptr<sf::Text> levelCompleteText;
    std::unique_ptr<sf::Text> levelCompleteButtonText;
    void processEvents();
    void update(float dt);
    void render();
    void handleCollisions(float dt);
    void resetPlayer();
}; 