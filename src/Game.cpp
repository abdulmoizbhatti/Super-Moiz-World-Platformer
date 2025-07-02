#include "Game.hpp"
#include "Constants.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

Game::Game() : window(sf::VideoMode(sf::Vector2u(GAME_WIDTH, GAME_HEIGHT)), "Simple C++ Platformer", sf::Style::Default) {
    window.setFramerateLimit(60);
    player = std::make_unique<Player>(GAME_WIDTH / 2, GAME_HEIGHT / 2);
    // Setup platforms directly for now
    platforms.emplace_back(sf::Vector2f(GAME_WIDTH, 50.0f), sf::Vector2f(0, GAME_HEIGHT - 50.0f));
    platforms.emplace_back(sf::Vector2f(200.0f, 30.0f), sf::Vector2f(100.0f, GAME_HEIGHT - 200.0f));
    platforms.emplace_back(sf::Vector2f(150.0f, 30.0f), sf::Vector2f(GAME_WIDTH - 300.0f, GAME_HEIGHT - 350.0f));
    platforms.emplace_back(sf::Vector2f(120.0f, 30.0f), sf::Vector2f(GAME_WIDTH / 2.0f - 60.0f, GAME_HEIGHT - 500.0f));
}

void Game::run() {
    sf::Clock deltaClock;
    while (window.isOpen()) {
        float dt = deltaClock.restart().asSeconds();
        processEvents();
        update(dt);
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
    // Draw background
    sf::RectangleShape background(sf::Vector2f(GAME_WIDTH, GAME_HEIGHT));
    background.setFillColor(sf::Color(135, 206, 235));
    window.draw(background);
    // Draw platforms
    for (auto& platform : platforms) {
        platform.draw(window);
    }
    // Draw player
    player->draw(window);
    window.display();
}

void Game::handleCollisions(float dt) {
    sf::FloatRect playerBounds = player->shape.getGlobalBounds();
    for (auto& platform : platforms) {
        sf::FloatRect platformBounds = platform.shape.getGlobalBounds();
        if (playerBounds.findIntersection(platformBounds).has_value()) {
            float previousPlayerBottom = player->shape.getPosition().y + playerBounds.size.y / 2 - player->velocity.y * dt;
            if (previousPlayerBottom <= platformBounds.position.y) {
                player->shape.setPosition(sf::Vector2f(player->shape.getPosition().x, platformBounds.position.y - playerBounds.size.y / 2.0f));
                player->velocity.y = 0;
                player->onGround = true;
            }
            // add other collision branches for sides/bottom if needed
        }
    }
}

void Game::resetPlayer() {
    player->shape.setPosition(sf::Vector2f(GAME_WIDTH / 2, GAME_HEIGHT / 2));
    player->velocity = sf::Vector2f(0, 0);
} 