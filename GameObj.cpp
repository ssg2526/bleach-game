#include "GameObj.h"
#include <iostream>
using namespace std;

GameObj::GameObj(){}

GameObj::GameObj(float x, float y, float w, float h, int obj_code,string objname){
	collisionBox.x = x;
	collisionBox.y = y;
	collisionBox.w = w;
	collisionBox.h = h;
	code = obj_code;
	name = objname;	
}

void GameObj::playerHitStatic(GameObj object){}
void GameObj::enemyHitStatic(GameObj object){}
void GameObj::bulletHitEnemy(GameObj object){}
void GameObj::render(float x, float y, SDL_Rect* clip , SDL_RendererFlip flipType){}
void GameObj::free(){}
