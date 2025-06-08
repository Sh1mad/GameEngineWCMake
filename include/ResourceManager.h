#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

struct TextureInfo {
    sf::Texture texture;
    std::string filePath;
};

class ResourceManager {
public:
    // Загружает текстуру по пути и связывает с пользовательским именем
    static bool loadTexture(const std::string& name, const std::string& path);

    // Получает текстуру по имени
    static sf::Texture& getTexture(const std::string& name);

	// Получаем путь к текстуре по имени
	static const std::string& getPath(const std::string& name);

    // Удаляет текстуру по имени
    static void removeTexture(const std::string& name);

    // Возвращает все текстуры (по имени)
    static const std::unordered_map<std::string, TextureInfo>& getAllTextures();

private:
    // Хранилище: имя -> текстура
    static std::unordered_map<std::string, TextureInfo> textures;
};