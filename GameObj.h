#ifndef GAMEOBJ_H
#define GAMEOBJ_H
#include <iostream>
#include "Global.h"
using namespace std;
class GameObj{
	public:
		GameObj();
		GameObj(float x, float y, float w, float h, int obj_code ,string objname);
		SDL_Rect collisionBox;
		string name;
		int code;
		SDL_Rect renderingClip;
		SDL_RendererFlip flipType;
		virtual void playerHitStatic(GameObj object);
		virtual void enemyHitStatic(GameObj object);
		virtual void bulletHitEnemy(GameObj object);
		virtual void render(float x, float y, SDL_Rect* clip = NULL, SDL_RendererFlip flipType=SDL_FLIP_NONE);
		virtual void free();


	private:

};
#endif