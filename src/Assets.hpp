#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class Assets {
public:
    Assets();
    void load();
    sf::Texture& getTexture(const std::string& name);
private:
    std::map<std::string, sf::Texture> textures;
}; 