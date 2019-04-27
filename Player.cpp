#include "Global.h"
#include "Player.h"
#include <cmath>
#include <fstream>

using namespace std;

SDL_Renderer* gameRenderer;
const float GRAVITY = 9.81;

Player::Player(){}

Player::Player(float x, float y, string playername){
	PlayerSheetTexture = NULL;
	name = playername;
	isCollidingBelow = false;
	collisionBox.x = x;
	collisionBox.y = y;
	collisionBox.w = 30.0;
	collisionBox.h = 42.0;	
	xPrevPos = x;
	yPrevPos = y;
	maxVel = 2.0;
	jumpVel = 4.0;
	tempJvel = 0.0;
	jumpCount = 0;
	xDelPos =0.0;
	yDelPos =0.0;
	camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	initializeClips();
	renderingClip.x = run[0].x;
	renderingClip.y = run[0].y;
	renderingClip.w = run[0].w;
	renderingClip.h = run[0].h;
	flipType = SDL_FLIP_NONE;
}

Player::~Player(){
	free();
}


void Player::initializeClips(){
	ifstream in;
	string act;
	SDL_Rect box;
	in.open("PlayerClips.txt");
	while(!in.eof()){
		in>>act;
		in>> box.x >> box.y >> box.w >> box.h;
		if(act == "run"){
			run.push_back(box);
		}
	}
}

void Player::render(float x, float y, SDL_Rect* clip, SDL_RendererFlip flipType){
	SDL_Rect spriteRect = {(int)x, (int)y, 0, 0};
	spriteRect.w = clip->w;
	spriteRect.h = clip->h;
	SDL_RenderCopyEx(gameRenderer, PlayerSheetTexture, clip, &spriteRect, 0, NULL, flipType);
}

void Player::handleMovement(SDL_Event e){
	switch(e.key.keysym.sym){
		case SDLK_LEFT:
			xDelPos -= (maxVel*TIME_STEP)*SCALE;
			flipType = SDL_FLIP_HORIZONTAL;
			//animateRun(true);
			//cout<<xDelPos<<" ";
		break;

		case SDLK_RIGHT:
			xDelPos += (maxVel*TIME_STEP)*SCALE;
			flipType = SDL_FLIP_NONE;
			//animateRun(true);
			
		break;

		case SDLK_UP:
			if(jumpCount<2){
				tempJvel = jumpVel;
				jumpCount++;
				//animateRun(false);
			}
			 
		break;

		case SDLK_DOWN:
		
		break;
	}
}

void Player::handleMovement(SDL_Event e, int button_released){
	switch(e.key.keysym.sym){
		case SDLK_LEFT:
			xDelPos += (maxVel*TIME_STEP)*SCALE;
			//animateRun(false);
		break;

		case SDLK_RIGHT:
			xDelPos -= (maxVel*TIME_STEP)*SCALE;
			//animateRun(false);
		break;

		case SDLK_UP:
			
		break;

		case SDLK_DOWN:
		
		break;
	}
}

void Player::updatePos(){
	xPrevPos = collisionBox.x;
	yPrevPos = collisionBox.y;
	yDelPos = ((-1*tempJvel*TIME_STEP + (0.5)*GRAVITY*TIME_STEP*TIME_STEP)*SCALE);
	tempJvel = tempJvel - (GRAVITY*TIME_STEP);
	collisionBox.x += xDelPos;
	collisionBox.y += yDelPos;
	if(collisionBox.x+collisionBox.w > LEVEL_WIDTH){
		collisionBox.x = LEVEL_WIDTH-collisionBox.w;
	}
	else if(collisionBox.x < 0){
		collisionBox.x = 0;
	}
	// cout<<collisionBox.y<<"		"<<yPrevPos<<endl;

	if(abs(xDelPos)>=EPSILON && isCollidingBelow==true){
		animateRun(true);
	}
	else{
		animateRun(false);
	}
	// isCollidingBelow = false;
}

void Player::playerHitStatic(GameObj object){
	if(yPrevPos + collisionBox.h <= (object).collisionBox.y){
		collisionBox.y = (object).collisionBox.y - collisionBox.h;
		yDelPos = 0;
		tempJvel = 0;
		jumpCount = 0;
		isCollidingBelow = true;
		// cout<<"b";
	}
	else{
		if(xPrevPos + collisionBox.w <= (object).collisionBox.x){
			collisionBox.x = (object).collisionBox.x - collisionBox.w;
		}
		else{
			if(xPrevPos >= (object).collisionBox.x + (object).collisionBox.w){
				collisionBox.x = (object).collisionBox.x + (object).collisionBox.w;
			}
			else{
				if(yPrevPos >= (object).collisionBox.y + (object).collisionBox.h){
					collisionBox.y = (object).collisionBox.y + (object).collisionBox.h;
					tempJvel = 0;	
				}
			}
		}
	}
}

void Player::animateRun(bool anim){
	static int i=0;
	renderingClip = run[i/4];
	//cout<<i<<endl;
	i++;
	if(i==16 || anim==false){
		i=0;
	}
}

void Player::free(){
	SDL_DestroyTexture(PlayerSheetTexture);
	PlayerSheetTexture = NULL;
}
