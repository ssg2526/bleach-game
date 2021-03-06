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
		void render(float x, float y, SDL_Rect* clip = NULL, SDL_RendererFlip flipType=SDL_FLIP_NONE);
		void handleMovement(SDL_Event e);
		void handleMovement(SDL_Event e, int button_released);
		void updatePos();
		void updatePos(int x, int y);
		void renderHealthBar();
		void bulletHitPlayer(GameObj obj);
		void playerHitStatic(GameObj object);
		int health;
		bool isCollidingBelow;
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
		SDL_Rect camera;
		void initializeClips();
		string prevAnim;
		void animate(string act);
		// void animateStand(bool anim);

};


#endif