#pragma once
#include "Entity.h"
#include <vector>
#include <memory>
#include <unordered_map>

class EntityManager {
public:
    // === Конструкторы и деструкторы ===
    EntityManager();
    ~EntityManager();

    // === Работа с сущностями ===
    void addEntity(std::shared_ptr<Entity> entity, std::string& name);
    void removeEntity(std::shared_ptr<Entity> entity);

    // === Получение данных ===
    int getNextId() const;
    std::vector<std::shared_ptr<Entity>> getEntities() const;

    // === Работа с ID ===
    bool isIdUsed(int id) const;
    void registerId(int id, const std::string& name);
    void unregisterId(int id);
    std::string getEntityName(int id) const;

	// Очистка списка сущностей
	void clear();

private:
    std::vector<std::shared_ptr<Entity>> entities; // Список сущностей
    std::unordered_map<int, std::string> entityIdMap; // ID <-> имя сущности
    mutable int lastAssignedId = 0; // Последний выданный ID
};