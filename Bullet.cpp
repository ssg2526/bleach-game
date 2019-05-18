#include "Global.h"
#include "Bullet.h"
#include <cmath>
#include <fstream>

using namespace std;

Bullet::Bullet(float x, float y, string obj_name, SDL_RendererFlip flip){
	name = obj_name;
	startPos = (int)x;
	lifeSpan = 600;
	collisionBox.x = x;
	collisionBox.y = y;
	collisionBox.w = 16.0;
	collisionBox.h = 10.0;
	if(flip == SDL_FLIP_NONE){
		maxVel = 6;	
	}
	else{
		maxVel = -6;
	}
}

void Bullet::updatePos(){
	collisionBox.x += maxVel*TIME_STEP*SCALE;
}

void Bullet::bulletHitEnemy(GameObj obj){
	// bullet.maxVel = 0;
	// cout<<"HIT ";
	// delete this;
}