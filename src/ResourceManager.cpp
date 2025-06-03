#include "ResourceManager.h"
#include <iostream>
#include <stdexcept>

// Инициализация статического поля
std::unordered_map<std::string, sf::Texture> ResourceManager::textures;

sf::Texture& ResourceManager::getTexture(const std::string& path) {
	auto it = textures.find(path);

	if (it != textures.end()) return it->second;   // Возвращаем существующую текстуру

	// Текстура еще не загружена - загружаем
	sf::Texture texture;
	if (!texture.loadFromFile(path)) {
		throw std::runtime_error("Unable to load a texture with this way: " + path);
	}

	// Сохраняем в кэш и возвращаем
	auto result = textures.emplace(path, std::move(texture));
	return result.first->second;
}