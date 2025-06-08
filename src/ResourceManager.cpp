#include "ResourceManager.h"

std::unordered_map<std::string, TextureInfo> ResourceManager::textures;

bool ResourceManager::loadTexture(const std::string& name, const std::string& path) {
    sf::Texture sfmlTexture;
    if (!sfmlTexture.loadFromFile(path))
        return false;

    TextureInfo info;
    info.texture = std::move(sfmlTexture);
    info.filePath = path;

    auto result = textures.emplace(name, std::move(info));
    return result.second;
}

sf::Texture& ResourceManager::getTexture(const std::string& name) {
    auto it = textures.find(name);
    if (it == textures.end()) {
        throw std::runtime_error("Текстура не найдена: " + name);
    }
    return it->second.texture;
}

void ResourceManager::removeTexture(const std::string& name) {
    textures.erase(name);
}

const std::string& ResourceManager::getPath(const std::string& name) {
    auto it = textures.find(name);
    if (it == textures.end()) {
        static std::string empty = "";
        return empty;
    }
    return it->second.filePath;
}

const std::unordered_map<std::string, TextureInfo>& ResourceManager::getAllTextures() {
    return textures;
}