#ifndef GAMEOBJ_H
#define GAMEOBJ_H
#include <iostream>
#include "Global.h"
using namespace std;
class GameObj{
	public:
		GameObj();
		GameObj(float x, float y, float w, float h, string objname);
		SDL_Rect collisionBox;
		string name;
		virtual void playerHitStatic(GameObj object);
		virtual void enemyHitStatic(GameObj object);

	private:

};
#endif