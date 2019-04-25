#ifndef ENEMY_H
#define ENEMY_H
#include <iostream>
#include "Global.h"
#include "GameObj.h"
#include "Player.h"
class Enemy : public GameObj{
	public:
		Enemy();
		Enemy(float x, float y, std::string enemyname);
		~Enemy();
		bool loadEnemyFromFile(std::string path);
		void render(float x, float y, SDL_Rect* clip = NULL, SDL_RendererFlip flipType=SDL_FLIP_NONE);
		//int getWidth();
		//int getHeight();
		//void handleMovement(SDL_Event e);
		//void handleMovement(SDL_Event e, int button_released);
		void updatePos(SDL_Rect playerCollisionBox);
		void enemyHitStatic(GameObj object);
		void free();
		bool isCollidingBelow;
		float xPrevPos;
		float yPrevPos;
		float maxVel;
		float jumpVel;
		float tempJvel;
		int jumpCount;
		float xDelPos;
		float yDelPos;
		SDL_RendererFlip flipType;
		vector<SDL_Rect> run;
		vector<SDL_Rect> jump;
		SDL_Rect renderingClip;
		//SDL_Rect camera;
		void initializeClips();
		void animateRun(bool anim);
	private:
		int EnemySheetWidth;
		int EnemySheetHeight;
		SDL_Texture* EnemySheetTexture;
};
#endif