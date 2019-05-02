#ifndef BULLET_H
#define BULLET_H
#include <iostream>
#include <string.h>
#include <vector>
#include "Global.h"
#include "GameObj.h"

class Bullet : public GameObj{
	public:
		Bullet(float x, float y, string obj_name, SDL_RendererFlip);
		float xPrevPos;
		float yPrevPos;
		float maxVel;
		int startPos;
		int lifeSpan;
		void updatePos();
		void bulletHitEnemy(GameObj obj);
};

#endif