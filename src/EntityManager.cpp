#include "EntityManager.h"

void EntityManager::addEntity(std::shared_ptr<Entity> entity) {
	entities.push_back(entity);
}

void EntityManager::updateAll(float deltaTime) {
	for (auto& entity : entities) {
		entity->update(deltaTime);
	}
}

void EntityManager::removeEntity(std::shared_ptr<Entity> entity) {
	entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
}

std::vector<std::shared_ptr<Entity>> EntityManager::getEntities() const {
	return entities;
}