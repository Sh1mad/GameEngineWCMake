#pragma once
#ifndef RESOURCE_MANAGER_H
#define RESOURSE_MANAGER_H

#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>

class ResourceManager {
public:
	// Геттер текстуры
	static sf::Texture& getTexture(const std::string& path);

private:
	// Хранилище текстур
	static std::unordered_map<std::string, sf::Texture> textures;

	// Приватный конструктор
	ResourceManager() = default;
};

#endif // !RESOURCE_MANAGER_H
