#include "Game.hpp"
#include "Constants.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <cmath>

Game::Game() : window(sf::VideoMode(sf::Vector2u(GAME_WIDTH, GAME_HEIGHT)), "C++ Platformer", sf::Style::Default) {
    window.setFramerateLimit(60);
    
    // Load Tiled map first to get map dimensions
    if (!tiledMap.load("maps/level_one.tmx")) {
        std::cerr << "Failed to load Tiled map!" << std::endl;
        // Fallback to center if map fails to load
        player = std::make_unique<Player>(GAME_WIDTH / 2, GAME_HEIGHT / 2);
    } else {
        // Position player at the left edge of the map
        const auto& mapSize = tiledMap.getTileCount();
        const auto& tileSize = tiledMap.getTileSize();
        float mapPixelHeight = mapSize.y * tileSize.y;
        float yOffset = GAME_HEIGHT - mapPixelHeight;
        
        // Start at left edge of map, slightly above ground level
        float startX = tileSize.x * 2; // 2 tiles from left edge
        float startY = mapPixelHeight + yOffset - 100; // Above ground level
        player = std::make_unique<Player>(startX, startY);
    }
    
    // Load background
    if (!backgroundTexture.loadFromFile("background/craftpix-402033-free-horizontal-2d-game-backgrounds/PNG/game_background_3/game_background_3.1.png")) {
        std::cerr << "Failed to load background image!" << std::endl;
    }
    backgroundSprite = std::make_unique<sf::Sprite>(backgroundTexture);
    auto bounds = backgroundSprite->getLocalBounds();
    backgroundSprite->setScale(sf::Vector2f(
        GAME_WIDTH / bounds.size.x,
        GAME_HEIGHT / bounds.size.y
    ));
    // Load all tileset textures
    tilesetTextures.clear();
    const auto& tilesets = tiledMap.getTilesets();
    for (const auto& ts : tilesets) {
        auto tex = std::make_unique<sf::Texture>();
        if (!tex->loadFromFile(ts.getImagePath())) {
            std::cerr << "Failed to load tileset image: " << ts.getImagePath() << std::endl;
        }
        tilesetTextures.push_back(std::move(tex));
    }
    // Clear tileSprites before loading
    tileSprites.clear();
    // For each tile layer
    const auto& mapSize = tiledMap.getTileCount();
    const auto& tileSize = tiledMap.getTileSize();
    float mapPixelHeight = mapSize.y * tileSize.y;
    float yOffset = GAME_HEIGHT - mapPixelHeight;
    for (const auto& layer : tiledMap.getLayers()) {
        if (layer->getType() == tmx::Layer::Type::Tile) {
            const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
            const auto& tiles = tileLayer.getTiles();
            for (std::size_t y = 0; y < mapSize.y; ++y) {
                for (std::size_t x = 0; x < mapSize.x; ++x) {
                    std::size_t idx = x + y * mapSize.x;
                    std::uint32_t rawID = tiles[idx].ID;
                    if (rawID == 0) continue; // empty tile
                    int tilesetIndex = -1;
                    for (int i = int(tilesets.size()) - 1; i >= 0; --i) {
                        if (rawID >= tilesets[i].getFirstGID()) {
                            tilesetIndex = i;
                            break;
                        }
                    }
                    if (tilesetIndex == -1) continue; // skip if not found
                    const auto& ts = tilesets[tilesetIndex];
                    sf::Texture* tex = tilesetTextures[tilesetIndex].get();
                    std::uint32_t tileID = rawID - ts.getFirstGID();
                    if (tileID >= ts.getTileCount()) continue;
                    int columns = tex->getSize().x / tileSize.x;
                    int tu = tileID % columns;
                    int tv = tileID / columns;
                    sf::Sprite sprite(*tex);
                    sprite.setTextureRect(sf::IntRect(
                        sf::Vector2i(tu * tileSize.x, tv * tileSize.y),
                        sf::Vector2i(tileSize.x, tileSize.y)
                    ));
                    sprite.setPosition(sf::Vector2f(x * tileSize.x, y * tileSize.y + yOffset));
                    tileSprites.push_back(sprite);
                }
            }
        }
    }
}

void Game::run() {
    sf::View view(sf::FloatRect(sf::Vector2f(0.f, 0.f), sf::Vector2f(GAME_WIDTH, GAME_HEIGHT)));
    sf::Clock deltaClock;
    while (window.isOpen()) {
        float dt = deltaClock.restart().asSeconds();
        processEvents();
        update(dt);
        // Camera follows player worldX
        view.setCenter(sf::Vector2f(player->worldX, GAME_HEIGHT / 2));
        window.setView(view);
        render();
    }
}

void Game::processEvents() {
    if (auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
    }
}

void Game::update(float dt) {
    player->handleInput();
    player->update(dt);
    player->onGround = false;
    handleCollisions(dt);
    if (player->shape.getPosition().y > GAME_HEIGHT + 100) {
        resetPlayer();
    }
}

void Game::render() {
    window.clear();
    float cameraX = player->worldX;
    // Draw background (offset by camera)
    if (backgroundSprite) {
        backgroundSprite->setPosition(sf::Vector2f(cameraX - GAME_WIDTH / 2, 0));
        window.draw(*backgroundSprite);
    }
    // Draw Tiled map tiles
    for (const auto& sprite : tileSprites) {
        window.draw(sprite);
    }
    // Draw player (already centered)
    player->draw(window);
    window.display();
}

void Game::handleCollisions(float dt) {
    // Get tile size and map size
    const auto& mapSize = tiledMap.getTileCount();
    const auto& tileSize = tiledMap.getTileSize();
    float mapPixelHeight = mapSize.y * tileSize.y;
    float yOffset = GAME_HEIGHT - mapPixelHeight;

    // Vertical collision (ground/platforms)
    float playerLeft = player->worldX - player->shape.getSize().x / 2;
    float playerRight = player->worldX + player->shape.getSize().x / 2;
    float playerBottom = player->worldY + player->shape.getSize().y / 2;
    int tileXLeft = static_cast<int>(playerLeft / tileSize.x);
    int tileXRight = static_cast<int>((playerRight - 1) / tileSize.x);
    int tileY = static_cast<int>((playerBottom - yOffset) / tileSize.y);

    player->onGround = false;
    for (const auto& layer : tiledMap.getLayers()) {
        if (layer->getType() == tmx::Layer::Type::Tile) {
            std::string lname = layer->getName();
            if (lname != "ground" && lname != "floating platforms") continue;
            const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
            const auto& tiles = tileLayer.getTiles();
            for (int x = tileXLeft; x <= tileXRight; ++x) {
                if (x < 0 || x >= static_cast<int>(mapSize.x) || tileY < 0 || tileY >= static_cast<int>(mapSize.y)) continue;
                std::size_t idx = x + tileY * mapSize.x;
                if (tiles[idx].ID != 0) {
                    // Snap player to top of tile (player's bottom should be at tile's top)
                    player->worldY = tileY * tileSize.y + yOffset - player->shape.getSize().y / 2;
                    player->velocity.y = 0;
                    player->onGround = true;
                    player->shape.setPosition(sf::Vector2f(player->worldX, player->worldY));
                    break;
                }
            }
        }
    }

    // Horizontal collision (walls)
    float playerTop = player->worldY - player->shape.getSize().y / 2;
    float playerBottom2 = player->worldY + player->shape.getSize().y / 2 - 1;
    int tileYTop = static_cast<int>((playerTop - yOffset) / tileSize.y);
    int tileYBottom = static_cast<int>((playerBottom2 - yOffset) / tileSize.y);
    // Check left
    int tileXLeftWall = static_cast<int>((playerLeft + player->velocity.x * dt) / tileSize.x);
    for (const auto& layer : tiledMap.getLayers()) {
        if (layer->getType() == tmx::Layer::Type::Tile) {
            std::string lname = layer->getName();
            if (lname != "ground" && lname != "floating platforms") continue;
            const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
            const auto& tiles = tileLayer.getTiles();
            for (int y = tileYTop; y <= tileYBottom; ++y) {
                if (tileXLeftWall < 0 || tileXLeftWall >= static_cast<int>(mapSize.x) || y < 0 || y >= static_cast<int>(mapSize.y)) continue;
                std::size_t idx = tileXLeftWall + y * mapSize.x;
                if (tiles[idx].ID != 0 && player->velocity.x < 0) {
                    // Block left movement
                    player->velocity.x = 0;
                    player->worldX = (tileXLeftWall + 1) * tileSize.x + player->shape.getSize().x / 2;
                    player->shape.setPosition(sf::Vector2f(player->worldX, player->worldY));
                    break;
                }
            }
        }
    }
    // Check right
    int tileXRightWall = static_cast<int>((playerRight + player->velocity.x * dt - 1) / tileSize.x);
    for (const auto& layer : tiledMap.getLayers()) {
        if (layer->getType() == tmx::Layer::Type::Tile) {
            std::string lname = layer->getName();
            if (lname != "ground" && lname != "floating platforms") continue;
            const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
            const auto& tiles = tileLayer.getTiles();
            for (int y = tileYTop; y <= tileYBottom; ++y) {
                if (tileXRightWall < 0 || tileXRightWall >= static_cast<int>(mapSize.x) || y < 0 || y >= static_cast<int>(mapSize.y)) continue;
                std::size_t idx = tileXRightWall + y * mapSize.x;
                if (tiles[idx].ID != 0 && player->velocity.x > 0) {
                    // Block right movement
                    player->velocity.x = 0;
                    player->worldX = tileXRightWall * tileSize.x - player->shape.getSize().x / 2;
                    player->shape.setPosition(sf::Vector2f(player->worldX, player->worldY));
                    break;
                }
            }
        }
    }
}

void Game::resetPlayer() {
    // Reset to the same starting position as constructor
    const auto& mapSize = tiledMap.getTileCount();
    const auto& tileSize = tiledMap.getTileSize();
    float mapPixelHeight = mapSize.y * tileSize.y;
    float yOffset = GAME_HEIGHT - mapPixelHeight;
    
    float startX = tileSize.x * 2; // 2 tiles from left edge
    float startY = mapPixelHeight + yOffset - 100; // Above ground level
    
    player->worldX = startX;
    player->worldY = startY;
    player->shape.setPosition(sf::Vector2f(player->worldX, player->worldY));
    player->velocity = sf::Vector2f(0, 0);
} 