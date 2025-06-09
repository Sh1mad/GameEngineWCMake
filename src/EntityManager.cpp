#include "EntityManager.h"

EntityManager::EntityManager() = default;
EntityManager::~EntityManager() = default;

// Добавление сущности с автоматическим назначением ID
void EntityManager::addEntity(std::shared_ptr<Entity> entity, std::string& name) {
    int id = entity->getId(); // предполагаем, что у Entity есть setId()
    
    if (id == -1 || isIdUsed(id)) { // если ID не задан или уже занят
        id = getNextId(); // получаем новый
        entity->setId(id); // устанавливаем
    }

    registerId(id, name); // сохраняем соответствие
    entities.push_back(entity);
}

// Удаление сущности по указателю
void EntityManager::removeEntity(std::shared_ptr<Entity> entity) {
    auto it = std::find(entities.begin(), entities.end(), entity);
    if (it != entities.end()) {
        int id = (*it)->getId();
        entities.erase(it);
        unregisterId(id);
    }
}

// Получение следующего свободного ID
int EntityManager::getNextId() const {
    // Проверяем ID от 1 до N
    for (int id = 1;; ++id) {
        if (!isIdUsed(id)) {
            return id;
        }
    }
}

// Проверка, используется ли ID
bool EntityManager::isIdUsed(int id) const {
    return entityIdMap.find(id) != entityIdMap.end();
}

// Регистрация ID + имени
void EntityManager::registerId(int id, const std::string& name) {
    entityIdMap[id] = name;
}

// Удаление ID из маппинга
void EntityManager::unregisterId(int id) {
    entityIdMap.erase(id);
}

// Получение имени по ID
std::string EntityManager::getEntityName(int id) const {
    auto it = entityIdMap.find(id);
    if (it == entityIdMap.end())
        return "";
    return it->second;
}

// Геттер списка сущностей
std::vector<std::shared_ptr<Entity>> EntityManager::getEntities() const {
    return entities;
}

// Очистка списка сущностей
void EntityManager::clear(){
	entities.clear();
	entityIdMap.clear();
}