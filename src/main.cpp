#include <iostream>
#include "WindowManager.h"
#include "Renderer.h"
#include "InputManager.h"
#include "Physics.h"
#include "Entity.h"
#include "EntityManager.h"
#include "ResourceManager.h"
#include <filesystem>

int test() {
    std::cout << "Рабочая директория: " << std::filesystem::current_path().string() << std::endl;

    try {
        WindowManager windowManager;
        Renderer renderer;
        InputManager inputManager;
        PhysicsManager physics;
        EntityManager entityManager;

        // Создание окна
        windowManager.createWindow("Simple Game", 800, 600);
        renderer.setWindow(windowManager.getWindowPtr());
        inputManager.setWindow(windowManager.getWindowPtr());

        // Создание объектов через ResourceManager
        auto& playerTexture = ResourceManager::getTexture("assets/textures/player.png");
        auto& platformTexture = ResourceManager::getTexture("assets/textures/platform.png");

        auto player = std::make_shared<Entity>(playerTexture, 100, 100, 0, 0);
        player->makeMoveable();
        auto platform = std::make_shared<Entity>(platformTexture, 0, 500, 0, 0);
        platform->makeStatic();

        entityManager.addEntity(player);
        entityManager.addEntity(platform);

        physics.setGravity(1000.0f, GravityDirection::Down);

        sf::Clock clock;

        // Главный игровой цикл
        while (windowManager.isWindowOpen()) {
            sf::Event event;
            while (windowManager.getWindow().pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    windowManager.closeWindow();

                inputManager.pollEvents(event);
            }

            float deltaTime = clock.restart().asSeconds();

            // Логика управления игроком
            if (player->checkMoveable()) {
                sf::Vector2f speed = player->getSpeed();

                if (inputManager.isKeyPressed(sf::Keyboard::A))
                    speed.x = -300;
                else if (inputManager.isKeyPressed(sf::Keyboard::D))
                    speed.x = 300;
                else
                    speed.x = 0;

                static bool spacePressed = false;

                if (inputManager.isKeyPressed(sf::Keyboard::Space)) {
                    // Однократный прыжок (проверяется через isOnGround)
                    if (!spacePressed) {
                        speed.y = -500;
                        spacePressed = true;
                    }
                }
                else {
                    spacePressed = false;
                }

                player->setSpeed(speed);
            }

            // Физика
            for (auto& entity : entityManager.getEntities()) {
                physics.applyGravity(*entity, deltaTime);
                physics.moveObject(*entity, deltaTime);

                for (auto& other : entityManager.getEntities()) {
                    if (entity != other) {
                        physics.handleCollision(*entity, *other);
                    }
                }
            }

            // Отрисовка
            renderer.clear();
            for (auto& entity : entityManager.getEntities()) {
                if (sf::Sprite* sprite = entity->getSprite()) {
                    renderer.drawSprite(*sprite);
                }
            }
            renderer.display();
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}