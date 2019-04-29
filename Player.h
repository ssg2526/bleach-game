#ifndef PLAYER_H
#define PLAYER_H
#include <iostream>
#include <string.h>
#include <vector>
#include "Global.h"
#include "GameObj.h"
class Player : public GameObj{
	public:
		Player();
		Player(float x, float y, int obj_code,std::string playername);
		~Player();
		void render(float x, float y, SDL_Rect* clip = NULL, SDL_RendererFlip flipType=SDL_FLIP_NONE);
		void handleMovement(SDL_Event e);
		void handleMovement(SDL_Event e, int button_released);
		void updatePos();
		void playerHitStatic(GameObj object);
		void free();
		// int code;
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
		SDL_Rect camera;
		void initializeClips();
		void animateRun(bool anim);
};
#endif