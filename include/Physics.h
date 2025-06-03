#pragma once
#ifndef PHYSICS_H
#define PHYSICS_H

#include <SFML/Graphics.hpp>
#include "Entity.h"

enum class CollisionType {
	None,       // Нет столкновения
	Top,        // Столкновение сверху
	Bottom,     // Столкновение снизу
	Left,       // Столкновение слева
	Right       // Столкновение справа
};

enum class GravityDirection {
	Upper,     // Гравитация действует вверх
	Down,      // Гравитация действует вниз (по умолчанию)
	Left,	   // Гравитация действует влево
	Right	   // Гравитация действует вправо
};

class PhysicsManager {
public:
	// Конструктор
	PhysicsManager() = default;

	// Применение гравитации к объекту
	void applyGravity(Entity& object, float deltaTime);

	// Перемещение объекта
	void moveObject(Entity& a, float deltaTime);

	void handleCollision(Entity& a, Entity& b);

	// Установка параметров гравитации
	void setGravity(float gravityParameter, GravityDirection direction = GravityDirection::Down);

private:
	float gravity = 1.0f;


	// Проверка столкновения между двумя объектами
	CollisionType checkCollision(Entity& a, Entity& b) const;
	GravityDirection gravityDirection = GravityDirection::Down;
};

#endif // !PHYSICS_H
