#include "ProjectManager.h"
#include "ResourceManager.h"
#include <fstream>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;
using json = nlohmann::json;

ProjectManager::ProjectManager(const std::string& projectDir)
    : projectDir(projectDir) {}

// Создание нового проекта
bool ProjectManager::createNewProject(const std::string& projectName, int width, int height, bool fullscreen) {
    currentProject.name = projectName;
    currentProject.windowWidth = width;
    currentProject.windowHeight = height;
    currentProject.fullscreen = fullscreen;
    currentProject.assetsPath = projectDir + "/assets";

    if (!createProjectDirectoryStructure(currentProject.assetsPath)) {
        std::cerr << "Unable to create structure of project." << std::endl;
        return false;
    }

    return saveProjectToFile(projectDir + "/project.json");
}

// Открытие существующего проекта
bool ProjectManager::openProject(const std::string& path) {
    if (!fs::exists(path)) {
        std::cerr << "!!! File of project wasn't found !!!" << path << std::endl;
        return false;
    }

    json projectJson = readJsonFile(path);
    if (projectJson.is_null()) {
        std::cerr << "!!! Error reading project file !!!" << std::endl;
        return false;
    }

    currentProject.name = projectJson.value("name", "Unnamed Project");
    currentProject.windowWidth = projectJson["window"].value("width", 800);
    currentProject.windowHeight = projectJson["window"].value("height", 600);
    currentProject.fullscreen = projectJson["window"].value("fullscreen", false);
    currentProject.assetsPath = projectJson.value("assets_path", "assets");

    // === Восстанавливаем текстуры ===
    if (projectJson.contains("textures")) {
        for (const auto& textureJson : projectJson["textures"]) {
            std::string name = textureJson.value("name", "");
            std::string path = textureJson.value("path", "");

            if (name.empty() || path.empty())
                continue;

            try {
                // Путь относительно assetsPath
                std::string fullPath = currentProject.assetsPath + "/" + path;
                ResourceManager::loadTexture(name, fullPath); // ← Загружаем текстуру по имени
            } catch (const std::exception& e) {
                std::cerr << "Ошибка при загрузке текстуры '" << name << "': " << e.what() << std::endl;
            }
        }
    }
    
    entityManager.clear(); // Очищаем текущие сущности

    if (projectJson.contains("entities")) {
        for (const auto& entityJson : projectJson["entities"]) {
            if (!entityJson.is_object()) {
                std::cerr << "Ошибка: элемент в 'entities' не является JSON-объектом" << std::endl;
                continue;
            }

            int id = entityJson.value("id", -1);
            if (id == -1) {
                std::cerr << "Ошибка: у сущности отсутствует ID." << std::endl;
                continue;
            }

            // Проверяем, есть ли position и speed
            if (!entityJson.contains("position") || !entityJson["position"].is_array() ||
                entityJson["position"].size() < 2) {
                std::cerr << "Ошибка: некорректное поле position для сущности ID=" << id << std::endl;
                continue;
            }

            float x = entityJson["position"][0];
            float y = entityJson["position"][1];
            float vx = 0, vy = 0;

            if (entityJson.contains("speed") && entityJson["speed"].is_array() && entityJson["speed"].size() >= 2) {
                vx = entityJson["speed"][0];
                vy = entityJson["speed"][1];
            }

            // Получаем имя текстуры
            std::string textureName = entityJson.value("texture_name", "");
            if (textureName.empty()) {
                std::cerr << "Ошибка: у сущности ID=" << id << " отсутствует имя текстуры" << std::endl;
                continue;
            }

            bool moveable = entityJson.value("moveable", false);

            // Проверяем, есть ли texture_rect и он корректен
            sf::IntRect textureRect(0, 0, 32, 32); // Значение по умолчанию
            bool isTextureRect = false;
            if (entityJson.contains("texture_rect") && entityJson["texture_rect"].is_object()) {
                isTextureRect = true;
                const auto& rectJson = entityJson["texture_rect"];
                textureRect.left   = rectJson.value("left", 0);
                textureRect.top    = rectJson.value("top", 0);
                textureRect.width  = rectJson.value("width", 32);
                textureRect.height = rectJson.value("height", 32);
            } else {
                std::cerr << "Предупреждение: texture_rect отсутствует или повреждён для ID=" << id << std::endl;
            }

            // Создание Entity
            try {
                std::shared_ptr<Entity> entity;

                if (isTextureRect)
                    entity = std::make_shared<Entity>(textureName, textureRect, id, x, y, vx, vy);
                else
                    entity = std::make_shared<Entity>(textureName, id, x, y, vx, vy);

                if (moveable) {
                    entity->makeMoveable();
                } else {
                    entity->makeStatic();
                }

                entityManager.addEntity(entity);
            }
            catch (const std::exception& e) {
                std::cerr << "Не удалось создать Entity ID=" << id << ": " << e.what() << std::endl;
            }
        }
    }

    return true;
}

// Сохранение текущего проекта
bool ProjectManager::saveCurrentProject() {
    return saveProjectToFile(projectDir + "/project.json");
}

const ProjectManager::ProjectData& ProjectManager::getProjectInfo() const {
    return currentProject;
}

EntityManager& ProjectManager::getEntityManager() {
    return entityManager;
}

// === PRIVATE METHODS ===

bool ProjectManager::createProjectDirectoryStructure(const std::string& projectPath) {
    try {
        fs::create_directories(projectPath);
        fs::create_directories(projectPath + "/textures/");
        fs::create_directories(projectPath + "/sounds/");
        return true;
    } catch (const std::exception& e) {
        std::cerr << "!!! Mistake creating structure of project !!! " << e.what() << std::endl;
        return false;
    }
}

bool ProjectManager::saveProjectToFile(const std::string& filePath) {
    json projectJson;

    // Информация о проекте
    projectJson["name"] = currentProject.name;
    projectJson["window"]["width"] = currentProject.windowWidth;
    projectJson["window"]["height"] = currentProject.windowHeight;
    projectJson["window"]["fullscreen"] = currentProject.fullscreen;
    projectJson["assets_path"] = currentProject.assetsPath;

    // === Сохраняем текстуры ===
    const auto& allTextures = ResourceManager::getAllTextures();
    json texturesJson = json::array();

    for (const auto& [name, info] : allTextures) {
        json textureJson;
        textureJson["name"] = name;
        textureJson["path"] = info.filePath;
        texturesJson.push_back(textureJson);
    }

    // Список сущностей
    json entitiesJson = json::array();
    for (auto& entity : entityManager.getEntities()) {
        json entJson;
        entJson["id"] = entity->getId();
        entJson["position"] = {entity->getPosition().x, entity->getPosition().y};
        entJson["speed"] = {entity->getSpeed().x, entity->getSpeed().y};
        entJson["texture_name"] = entity->getTextureName(); // Имя текстуры
        entJson["moveable"] = entity->checkMoveable();

        // Сохраняем регион текстуры
        const sf::IntRect& rect = entity->getTextureRect();
        entJson["texture_rect"] = {
            {"left", rect.left},
            {"top", rect.top},
            {"width", rect.width},
            {"height", rect.height}
        };

        entitiesJson.push_back(entJson);
    }

    projectJson["entities"] = entitiesJson;

    // Сохраняем в файл
    std::ofstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "!!! Unable to open file for writing... !!!" << filePath << std::endl;
        return false;
    }

    file << projectJson.dump(4);
    file.close();

    return true;
}

json ProjectManager::readJsonFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "!!! Unable to open file !!!" << filePath << std::endl;
        return nullptr; // или json()
    }

    nlohmann::json data;
    try {
        file >> data;

        if (data.is_null()) {
            std::cerr << "!!! File exists but is empty or invalid !!!" << std::endl;
            return nullptr;
        }

        if (!data.contains("name")) {
            std::cerr << "!!! File isn't correct !!!" << std::endl;
            return nullptr;
        }

        return data;

    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "!!! Parsing error !!!" << e.what() << std::endl;
        return nullptr;
    }
}

bool ProjectManager::writeJsonFile(const std::string& filePath, const json& data) {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "!!! Unable to open file !!!" << filePath << std::endl;
        return false;
    }

    file << data.dump(4);
    file.close();
    return true;
}