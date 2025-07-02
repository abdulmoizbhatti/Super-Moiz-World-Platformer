#include "Assets.hpp"

Assets::Assets() {}
void Assets::load() {}
sf::Texture& Assets::getTexture(const std::string& name) {
    // need to return texture by name
    static sf::Texture dummy;
    return dummy;
} 