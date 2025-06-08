#pragma once

#include <string>
#include <vector>
#include <memory>
#include <filesystem>

#include "Entity.h"
#include "EntityManager.h"

#include <json.hpp>

class ProjectManager {
public:
    struct ProjectData {
        std::string name;
        int windowWidth = 800;
        int windowHeight = 600;
        bool fullscreen = false;
        std::string assetsPath = "assets/";

        // Список текстур проекта: имя -> путь
        std::unordered_map<std::string, std::string> textures;
    };

    // Конструктор
    explicit ProjectManager(const std::string& projectDir = "./projects/");

    // Работа с проектами
    bool createNewProject(const std::string& projectName, int width, int height, bool fullscreen);
    bool openProject(const std::string& path);
    bool saveCurrentProject();

    // Получить данные проекта
    const ProjectData& getProjectInfo() const;

    // Доступ к сущностям
    EntityManager& getEntityManager();

private:
    ProjectData currentProject;
    EntityManager entityManager;
    std::string projectDir;

    // Вспомогательные методы
    bool saveProjectToFile(const std::string& filePath);
    bool loadProjectFromFile(const std::string& filePath);
    nlohmann::json readJsonFile(const std::string& filePath);
    bool writeJsonFile(const std::string& filePath, const nlohmann::json& data);
    

    // Создание структуры проекта
    bool createProjectDirectoryStructure(const std::string& projectPath);
};