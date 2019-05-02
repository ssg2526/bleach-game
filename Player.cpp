#include "Global.h"
#include "Player.h"
#include <cmath>
#include <fstream>

using namespace std;

SDL_Renderer* gameRenderer;
const float GRAVITY = 9.81;

Player::Player(){}

Player::Player(float x, float y, int obj_code, string playername){
	PlayerSheetTexture = NULL;
	name = playername;
	isCollidingBelow = false;
	collisionBox.x = x;
	collisionBox.y = y;
	collisionBox.w = 50.0;
	collisionBox.h = 48.0;	
	code = obj_code;
	xPrevPos = x;
	yPrevPos = y;
	maxVel = 4.0;
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
		run.push_back(box);
		// if(act == "run"){
		// 	run.push_back(box);
		// }
		// if(act=="stand"){
		// 	run.push_back(box);
		// }
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
		case SDLK_z:
			xDelPos = 0;

			BYPASS = true;
			// animate("shoot");
			// Bullet bullet(collisionBox.x+collisionBox.w, collisionBox.y+collisionBox.h/4);
		break;
		
	}
}

void Player::handleMovement(SDL_Event e, int button_released){
	switch(e.key.keysym.sym){
		case SDLK_LEFT:
			// xDelPos += (maxVel*TIME_STEP)*SCALE;
			xDelPos = 0;
		break;

		case SDLK_RIGHT:
			// xDelPos -= (maxVel*TIME_STEP)*SCALE;
			xDelPos = 0;
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
	// collisionBox.w = renderingClip.w;
	// collisionBox.h = renderingClip.h;
	if(collisionBox.x+collisionBox.w > LEVEL_WIDTH){
		collisionBox.x = LEVEL_WIDTH-collisionBox.w;
	}
	else if(collisionBox.x < 0){
		collisionBox.x = 0;
	}
	// cout<<collisionBox.y<<"		"<<yPrevPos<<endl;

	if(BYPASS){
			animate("shoot");
	}
	else if(abs(xDelPos)>=EPSILON && isCollidingBelow==true){
		animate("run");
	}
	
	else{
		// if(isCollidingBelow == true){
			animate("stand");
		// }
		// else{
		// 	animate("jump");
		// }
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

void Player::animate(string act){
	static int i=0;
	if(act == "run"){
		renderingClip = run[i/5];
		// cout<<i<<" ";
		i++;
		if(i==20){
			i=0;
		}
	}
	else if(act == "shoot"){
		// i=0;
		// cout<<11+i/4<<" ";
		renderingClip = run[12+i/5];
		i++;
		if(i>=15){
			i=0;
			BYPASS = false;
			// Bullet bullet(collisionBox.x + collisionBox.w, collisionBox.y+20);
		}
	}
	else if(act == "stand"){
		renderingClip = run[5];
		// i++;
		// collisionBox.h = renderingClip.h;
		// if(i>40){
		// 	i=0;
		// }
	}
	
	// else if(act == "jump"){
	// 	i=5;
	// 	renderingClip = run[i/4];
	// 	//cout<<i<<endl;
	// 	i++;
	// 	if(i==16){
	// 		i=5;
	// 	}
	// }
	
}


void Player::free(){
	SDL_DestroyTexture(PlayerSheetTexture);
	PlayerSheetTexture = NULL;
}


