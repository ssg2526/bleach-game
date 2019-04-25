#ifndef COLLISIONDETECTOR_H
#define COLLISIONDETECTOR_H
#include "Global.h"
#include "GameObj.h"
#include "Player.h"
#include "Enemy.h"

class CollisionDetector{
	public:
		CollisionDetector();
		void checkCollision(std::vector<GameObj*> &object);
};

#endif