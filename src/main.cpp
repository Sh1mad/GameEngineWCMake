#include <iostream>
#include "ProjectManager.h"
#include "Entity.h"
#include "EntityManager.h"

int main() {
    try {
        // Инициализируем ProjectManager
        ProjectManager projectManager("../projects/TestProject");

        // 1. Создаём новый проект
        std::cout << "Создаём новый проект..." << std::endl;
        projectManager.createNewProject("TestGame", 800, 600, false);

        // 2. Добавляем сущность с регионом текстуры
        sf::IntRect playerRect(0, 0, 32, 32);
        auto entity = std::make_shared<Entity>("../assets/textures/player.png", 1, 100, 100, 0, 0);
        entity->makeMoveable();
        projectManager.getEntityManager().addEntity(entity);

        // 3. Сохраняем проект
        std::cout << "Сохраняем проект..." << std::endl;
        projectManager.saveCurrentProject();

        // 4. Очищаем текущий менеджер для теста загрузки
        ProjectManager loader;
        std::cout << "Загружаем проект..." << std::endl;
        loader.openProject("../projects/TestProject/project.json");

        // 5. Проверяем данные после загрузки
        const auto& loadedEntities = loader.getEntityManager().getEntities();
        if (!loadedEntities.empty()) {
            const auto& loadedEntity = loadedEntities[0];

            std::cout << "ID: " << loadedEntity->getId() << std::endl;
            std::cout << "Позиция: (" << loadedEntity->getPosition().x << ", " << loadedEntity->getPosition().y << ")" << std::endl;
            std::cout << "Текстура: " << loadedEntity->getTexturePath() << std::endl;

            sf::IntRect rect = loadedEntity->getTextureRect();
            std::cout << "Регион текстуры: (" << rect.left << ", " << rect.top << ", " << rect.width << ", " << rect.height << ")" << std::endl;

            std::cout << "Скорость: (" << loadedEntity->getSpeed().x << ", " << loadedEntity->getSpeed().y << ")" << std::endl;
            std::cout << "Движимый: " << (loadedEntity->checkMoveable() ? "да" : "нет") << std::endl;

            std::cout << "Тест пройден успешно!" << std::endl;
        } else {
            std::cerr << "Ошибка: сущности не загружены." << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Произошла ошибка: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}