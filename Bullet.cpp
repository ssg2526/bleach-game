#include "Global.h"
#include "Bullet.h"
#include <cmath>
#include <fstream>

using namespace std;

Bullet::Bullet(float x, float y, string obj_name){
	name = obj_name;
	collisionBox.x = x;
	collisionBox.y = y;
	collisionBox.w = 16.0;
	collisionBox.h = 10.0;
	maxVel = 10;	
}

Bullet::~Bullet(){
	cout<<"Freed";
};

void Bullet::updatePos(){
	collisionBox.x += maxVel*TIME_STEP*SCALE;
	if(collisionBox.x >= LEVEL_WIDTH){
		collisionBox.x = 0;
	}
}

void Bullet::bulletHitEnemy(GameObj obj){
	// bullet.maxVel = 0;
	cout<<"HIT ";
	// delete this;
}