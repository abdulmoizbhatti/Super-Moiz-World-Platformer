#include "Game.hpp"
#include "Constants.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

Game::Game() : window(sf::VideoMode(sf::Vector2u(GAME_WIDTH, GAME_HEIGHT)), "C++ Platformer", sf::Style::Default) {
    window.setFramerateLimit(60);
    player = std::make_unique<Player>(GAME_WIDTH / 2, GAME_HEIGHT / 2);
    // Setup platforms directly for now
    platforms.emplace_back(sf::Vector2f(10000.0f, 50.0f), sf::Vector2f(-5000.0f, GAME_HEIGHT - 50.0f));
    platforms.emplace_back(sf::Vector2f(200.0f, 30.0f), sf::Vector2f(100.0f, GAME_HEIGHT - 200.0f));
    platforms.emplace_back(sf::Vector2f(150.0f, 30.0f), sf::Vector2f(GAME_WIDTH - 300.0f, GAME_HEIGHT - 350.0f));
    platforms.emplace_back(sf::Vector2f(120.0f, 30.0f), sf::Vector2f(GAME_WIDTH / 2.0f - 60.0f, GAME_HEIGHT - 500.0f));
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
    sf::RectangleShape background(sf::Vector2f(GAME_WIDTH, GAME_HEIGHT));
    background.setFillColor(sf::Color(135, 206, 235));
    background.setPosition(sf::Vector2f(cameraX - GAME_WIDTH / 2, 0));
    window.draw(background);
    // Draw a very wide ground for infinite movement
    sf::RectangleShape ground(sf::Vector2f(10000.0f, 50.0f));
    ground.setFillColor(sf::Color(139, 69, 19));
    ground.setOutlineColor(sf::Color::Black);
    ground.setOutlineThickness(2.0f);
    ground.setPosition(sf::Vector2f(-5000.0f - (cameraX - GAME_WIDTH / 2), GAME_HEIGHT - 50.0f));
    window.draw(ground);
    // Draw platforms (offset by camera)
    for (auto& platform : platforms) {
        sf::Vector2f worldPos = platform.shape.getPosition();
        sf::Vector2f drawPos = sf::Vector2f(worldPos.x - (cameraX - GAME_WIDTH / 2), worldPos.y);
        platform.shape.setPosition(drawPos);
        window.draw(platform.shape);
        platform.shape.setPosition(worldPos); // Restore original position
    }
    // Draw player (already centered)
    player->draw(window);
    window.display();
}

void Game::handleCollisions(float dt) {
    sf::FloatRect playerBounds(
        sf::Vector2f(player->worldX - player->shape.getSize().x / 2, player->worldY - player->shape.getSize().y / 2),
        player->shape.getSize()
    );
    for (auto& platform : platforms) {
        sf::Vector2f platPos = platform.shape.getPosition();
        sf::Vector2f platSize = platform.shape.getSize();
        sf::FloatRect platformBounds(sf::Vector2f(platPos.x, platPos.y), platSize);
        if (playerBounds.findIntersection(platformBounds).has_value()) {
            float previousPlayerBottom = player->worldY + player->shape.getSize().y / 2 - player->velocity.y * dt;
            if (previousPlayerBottom <= platformBounds.position.y) {
                player->worldY = platformBounds.position.y - player->shape.getSize().y / 2.0f;
                player->velocity.y = 0;
                player->onGround = true;
                player->shape.setPosition(sf::Vector2f(player->worldX, player->worldY));
            }
        }
    }
}

void Game::resetPlayer() {
    player->worldX = GAME_WIDTH / 2;
    player->worldY = GAME_HEIGHT / 2;
    player->shape.setPosition(sf::Vector2f(player->worldX, player->worldY));
    player->velocity = sf::Vector2f(0, 0);
} 