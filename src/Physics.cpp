#include "Physics.h"

void PhysicsManager::setGravity(float gravityParameter, GravityDirection direction) {
	gravity = gravityParameter;
	gravityDirection = direction;
}

// Применение гравитации к объекту
void PhysicsManager::applyGravity(Entity& object, float deltaTime) {
	if (!object.checkMoveable()) return;

	sf::Vector2f speed = object.getSpeed();

	if (gravityDirection == GravityDirection::Down)
		speed.y += gravity * deltaTime;
	else if (gravityDirection == GravityDirection::Upper)
		speed.y -= gravity * deltaTime;
	else if (gravityDirection == GravityDirection::Right)
		speed.x += gravity * deltaTime;
	else if (gravityDirection == GravityDirection::Left)
		speed.x -= gravity * deltaTime;

	object.setSpeed(speed);
}

// Перемещение объекта
void PhysicsManager::moveObject(Entity& a, float deltaTime) {
	sf::Sprite* sprite = a.getSprite();
	if (!sprite) return;

	sprite->move(a.getSpeed() * deltaTime);
	a.setPosition(sprite->getPosition());
}

//CollisionType PhysicsManager::checkCollision(Entity& a, Entity& b) const{
//	sf::RectangleShape* firstTexture = a.getObject(), *secondTexture = b.getObject();
//	if (!firstTexture->getGlobalBounds().intersects(secondTexture->getGlobalBounds()))
//		return CollisionType::None;
//	if (((a.getPosition().y + firstTexture->getSize().y) <= b.getPosition().y) &&
//		((firstTexture->getPosition().y + firstTexture->getSize().y) >= secondTexture->getPosition().y))
//		return CollisionType::Top;
//	if (((a.getPosition().x + firstTexture->getSize().x) <= b.getPosition().x) &&
//		((firstTexture->getPosition().x + firstTexture->getSize().x) >= secondTexture->getPosition().x))
//		return CollisionType::Left;
//	if ((a.getPosition().x >= (b.getPosition().x + secondTexture->getSize().x)) &&
//		(firstTexture->getPosition().x <= (secondTexture->getPosition().x + secondTexture->getSize().x)))
//		return CollisionType::Right;
//	if ((a.getPosition().y >= (b.getPosition().y + secondTexture->getSize().y)) &&
//		(firstTexture->getPosition().y <= (secondTexture->getPosition().y + secondTexture->getSize().y)))
//		return CollisionType::Bottom;
//}

// Проверка наличия столкновения и его направления
CollisionType PhysicsManager::checkCollision(Entity& a, Entity& b) const {
	sf::Sprite* spriteA = a.getSprite();
	sf::Sprite* spriteB = b.getSprite();

	if (!spriteA || !spriteB)
		return CollisionType::None;

	sf::FloatRect aBounds = spriteA->getGlobalBounds();
	sf::FloatRect bBounds = spriteB->getGlobalBounds();

	if (!aBounds.intersects(bBounds))
		return CollisionType::None;

	float halfWidthA = aBounds.width / 2.0f;
	float halfWidthB = bBounds.width / 2.0f;
	float halfHeightA = aBounds.height / 2.0f;
	float halfHeightB = bBounds.height / 2.0f;

	float centerAX = aBounds.left + halfWidthA;
	float centerAY = aBounds.top + halfHeightA;
	float centerBX = bBounds.left + halfWidthB;
	float centerBY = bBounds.top + halfHeightB;

	float dx = centerAX - centerBX;
	float dy = centerAY - centerBY;

	if (std::abs(dx) > std::abs(dy)) {
		return (dx > 0) ? CollisionType::Right : CollisionType::Left;
	}
	else {
		return (dy > 0) ? CollisionType::Bottom : CollisionType::Top;
	}
}

// Обработка столкновения
void PhysicsManager::handleCollision(Entity& a, Entity& b) {
	CollisionType type = checkCollision(a, b);
	sf::Sprite* spriteA = a.getSprite();
	sf::Sprite* spriteB = b.getSprite();

	if (!spriteA || !spriteB)
		return;

	sf::FloatRect aBounds = spriteA->getGlobalBounds();
	sf::FloatRect bBounds = spriteB->getGlobalBounds();

	switch (type) {
	case CollisionType::Top: {
		float newY = bBounds.top - aBounds.height;
		spriteA->setPosition(aBounds.left, newY);
		a.setPosition({ aBounds.left, newY });
		a.stopY();
		break;
	}
	case CollisionType::Bottom: {
		float newY = bBounds.top + bBounds.height;
		spriteA->setPosition(aBounds.left, newY);
		a.setPosition({ aBounds.left, newY });
		a.stopY();
		break;
	}
	case CollisionType::Left: {
		float newX = bBounds.left - aBounds.width;
		spriteA->setPosition(newX, aBounds.top);
		a.setPosition({ newX, aBounds.top });
		a.stopX();
		break;
	}
	case CollisionType::Right: {
		float newX = bBounds.left + bBounds.width;
		spriteA->setPosition(newX, aBounds.top);
		a.setPosition({ newX, aBounds.top });
		a.stopX();
		break;
	}
	default:
		break;
	}
}