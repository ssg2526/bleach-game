#ifndef ENEMY_H
#define ENEMY_H
#include <iostream>
#include "Global.h"
#include "GameObj.h"
#include "Player.h"
class Enemy : public GameObj{
	public:
		Enemy();
		Enemy(float x, float y, int obj_code, std::string enemyname);
		~Enemy();
		void render(float x, float y, SDL_Rect* clip = NULL, SDL_RendererFlip flipType=SDL_FLIP_NONE);
		void updatePos(SDL_Rect playerCollisionBox);
		void enemyHitStatic(GameObj object);
		void free();
		bool isCollidingBelow;
		// int code;
		float xPrevPos;
		float yPrevPos;
		float maxVel;
		float jumpVel;
		float tempJvel;
		int jumpCount;
		float xDelPos;
		float yDelPos;
		// SDL_RendererFlip flipType;
		vector<SDL_Rect> run;
		vector<SDL_Rect> jump;
		// SDL_Rect renderingClip;
		//SDL_Rect camera;
		void initializeClips();
		void animateRun(bool anim);
};
#endif