#pragma once
#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "Entity.h"
#include <vector>
#include <memory>

class EntityManager {
private:
	std::vector<std::shared_ptr<Entity>> entities;

public:
	// Создание новой сущности
	void addEntity(std::shared_ptr<Entity> entity);

	// Обновление всех сущностей
	void updateAll(float deltaTime);

	// Геттер списка сущностей
	std::vector<std::shared_ptr<Entity>> getEntities() const;

	// Удаление объекта
	void removeEntity(std::shared_ptr<Entity> entity);

	// Очистка сущностей
	void clear();

};

#endif // !ENTITY_MANAGER_H
