#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

// Game constants
const float GAME_WIDTH = 1024.0f;
const float GAME_HEIGHT = 768.0f;
const sf::Vector2f GRAVITY(0.0f, 1200.0f);
const float PLAYER_MOVE_SPEED = 350.0f;
const float PLAYER_JUMP_VELOCITY = -600.0f;

// A simple helper function to keep the view's aspect ratio correct
void updateView(sf::RenderWindow& window, sf::View& view) {
    float windowRatio = (float)window.getSize().x / (float)window.getSize().y;
    float viewRatio = view.getSize().x / view.getSize().y;
    float sizeX = 1.0f;
    float sizeY = 1.0f;
    float posX = 0.0f;
    float posY = 0.0f;

    bool isWider = windowRatio > viewRatio;
    if (isWider) {
        sizeX = viewRatio / windowRatio;
        posX = (1.0f - sizeX) / 2.0f;
    } else {
        sizeY = windowRatio / viewRatio;
        posY = (1.0f - sizeY) / 2.0f;
    }

    view.setViewport(sf::FloatRect(sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY)));
}

// Player
struct Player {
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    bool onGround;

    Player(float x, float y) {
        shape.setSize(sf::Vector2f(40.0f, 60.0f));
        shape.setOrigin(shape.getSize() / 2.0f);
        shape.setPosition(sf::Vector2f(x, y));
        shape.setFillColor(sf::Color(255, 87, 51));
        shape.setOutlineColor(sf::Color::Black);
        shape.setOutlineThickness(2.0f);
        onGround = false;
    }

    void handleInput() {
        // Horizontal movement
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            velocity.x = -PLAYER_MOVE_SPEED;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            velocity.x = PLAYER_MOVE_SPEED;
        } else {
            velocity.x = 0;
        }

        // Jumping
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) && onGround) {
            velocity.y = PLAYER_JUMP_VELOCITY;
            onGround = false;
        }
    }

    void update(float dt) {
        velocity += GRAVITY * dt;
        shape.move(velocity * dt);
    }
    
    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }
};

// Main game function
int main() {
    std::cout << "Starting SFML game..." << std::endl;
    
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(GAME_WIDTH, GAME_HEIGHT)), "Simple C++ Platformer", sf::Style::Default);
    window.setFramerateLimit(60);
    
    std::cout << "Window created successfully!" << std::endl;
    std::cout << "Window size: " << window.getSize().x << "x" << window.getSize().y << std::endl;

    sf::View view(sf::FloatRect(sf::Vector2f(0.f, 0.f), sf::Vector2f(GAME_WIDTH, GAME_HEIGHT)));
    
    Player player(GAME_WIDTH / 2, GAME_HEIGHT / 2);

    sf::RectangleShape background(sf::Vector2f(GAME_WIDTH, GAME_HEIGHT));
    background.setFillColor(sf::Color(135, 206, 235));

    std::vector<sf::RectangleShape> platforms;

    sf::RectangleShape floor;
    floor.setSize(sf::Vector2f(GAME_WIDTH, 50.0f));
    floor.setPosition(sf::Vector2f(0, GAME_HEIGHT - 50.0f));
    floor.setFillColor(sf::Color(34, 139, 34));
    floor.setOutlineColor(sf::Color::Black);
    floor.setOutlineThickness(2.0f);
    platforms.push_back(floor);

    sf::RectangleShape p1;
    p1.setSize(sf::Vector2f(200.0f, 30.0f));
    p1.setPosition(sf::Vector2f(100.0f, GAME_HEIGHT - 200.0f));
    p1.setFillColor(sf::Color(139, 69, 19));
    platforms.push_back(p1);

    sf::RectangleShape p2;
    p2.setSize(sf::Vector2f(150.0f, 30.0f));
    p2.setPosition(sf::Vector2f(GAME_WIDTH - 300.0f, GAME_HEIGHT - 350.0f));
    p2.setFillColor(sf::Color(139, 69, 19));
    platforms.push_back(p2);

    sf::RectangleShape p3;
    p3.setSize(sf::Vector2f(120.0f, 30.0f));
    p3.setPosition(sf::Vector2f(GAME_WIDTH / 2.0f - 60.0f, GAME_HEIGHT - 500.0f));
    p3.setFillColor(sf::Color(139, 69, 19));
    platforms.push_back(p3);

    sf::Clock deltaClock;
    
    std::cout << "Game initialized! Starting main loop..." << std::endl;
    std::cout << "Controls: A/D or Arrow Keys to move, W/Up/Space to jump" << std::endl;

    while (window.isOpen()) {
        float dt = deltaClock.restart().asSeconds();

        if (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                std::cout << "Window closed by user" << std::endl;
                window.close();
            }
            if (event->is<sf::Event::Resized>()) {
                updateView(window, view);
            }
        }

        player.handleInput();
        player.update(dt);
        player.onGround = false;

        sf::FloatRect playerBounds = player.shape.getGlobalBounds();

        for (auto& platform : platforms) {
            sf::FloatRect platformBounds = platform.getGlobalBounds();
            if (playerBounds.findIntersection(platformBounds).has_value()) {
                float previousPlayerBottom = player.shape.getPosition().y + playerBounds.size.y / 2 - player.velocity.y * dt;
                
                if (previousPlayerBottom <= platformBounds.position.y) {
                    player.shape.setPosition(sf::Vector2f(player.shape.getPosition().x, platformBounds.position.y - playerBounds.size.y / 2.0f));
                    player.velocity.y = 0;
                    player.onGround = true;
                }
                else if (player.shape.getPosition().y - playerBounds.size.y / 2 > platformBounds.position.y + platformBounds.size.y - 10) {
                     player.shape.setPosition(sf::Vector2f(player.shape.getPosition().x, platformBounds.position.y + platformBounds.size.y + playerBounds.size.y / 2.0f));
                     player.velocity.y = 0;
                }
                else {
                    if (player.shape.getPosition().x < platformBounds.position.x) {
                        player.shape.setPosition(sf::Vector2f(platformBounds.position.x - playerBounds.size.x / 2.0f, player.shape.getPosition().y));
                    } else {
                        player.shape.setPosition(sf::Vector2f(platformBounds.position.x + platformBounds.size.x + playerBounds.size.x / 2.0f, player.shape.getPosition().y));
                    }
                     player.velocity.x = 0;
                }
            }
        }

        if (player.shape.getPosition().y > GAME_HEIGHT + 100) {
            player.shape.setPosition(sf::Vector2f(GAME_WIDTH / 2, GAME_HEIGHT / 2));
            player.velocity = sf::Vector2f(0, 0);
        }

        window.clear();
        window.setView(view);

        window.draw(background);
        for (const auto& platform : platforms) {
            window.draw(platform);
        }
        player.draw(window);

        window.display();
    }
    
    std::cout << "Game ended normally" << std::endl;
    return 0;
}

