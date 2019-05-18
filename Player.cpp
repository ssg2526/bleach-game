#include "GameObj.h"
#include "Global.h"
#include "Bullet.h"
#include "Player.h"
#include <cmath>
#include <fstream>

using namespace std;
static int cn = 0;
vector<string> keys;
string prevAct;
string prevKeyPress;
SDL_Renderer* gameRenderer;
const double GRAVITY = 9.81;

Player::Player(){}

Player::Player(float x, float y, int obj_code, string playername){
	PlayerSheetTexture = NULL;
	name = playername;
	health = 100;
	isCollidingBelow = false;
	collisionBox.x = x;
	collisionBox.y = y;
	collisionBox.w = 70.0;
	collisionBox.h = 60.0;	
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



void Player::initializeClips(){
	ifstream in;
	string act;
	SDL_Rect box;
	in.open("PlayerClips.txt");
	while(!in.eof()){
		in>>act;
		in>> box.x >> box.y >> box.w >> box.h;
		run.push_back(box);
	}
}

void Player::renderHealthBar(){
	float percentHealth = health/100.0;
	int barWidth = 300;
	int barHeight = 15;
	SDL_Rect emptyHealth, fillHealth;
	SDL_SetRenderDrawColor( gameRenderer, 50, 205, 50, 255 );
	fillHealth.x = 20;
	fillHealth.y = 20;
	fillHealth.w = barWidth*percentHealth;
	fillHealth.h = barHeight;
	SDL_RenderFillRect(gameRenderer, &fillHealth);
	SDL_SetRenderDrawColor(gameRenderer, 255, 0, 0, 255);
	emptyHealth.x = fillHealth.x+fillHealth.w;
	emptyHealth.y = fillHealth.y;
	emptyHealth.w = barWidth*(1.0-percentHealth);
	emptyHealth.h = barHeight;
	SDL_RenderFillRect(gameRenderer, &emptyHealth);

}

void Player::render(float x, float y, SDL_Rect* clip, SDL_RendererFlip flipType){
	SDL_Rect spriteRect = {(int)x, (int)y, 0, 0};
	spriteRect.w = clip->w;
	spriteRect.h = clip->h;
	SDL_RenderCopyEx(gameRenderer, PlayerSheetTexture, clip, &spriteRect, 0, NULL, flipType);
	renderHealthBar();
}

void Player::handleMovement(SDL_Event e){
	switch(e.key.keysym.sym){
		case SDLK_LEFT:
			xDelPos -= (maxVel*TIME_STEP)*SCALE;
			flipType = SDL_FLIP_HORIZONTAL;
			prevKeyPress = "LD";
			keys.push_back(prevKeyPress);
		break;

		case SDLK_RIGHT:
			xDelPos += (maxVel*TIME_STEP)*SCALE;
			flipType = SDL_FLIP_NONE;
			prevKeyPress = "RD";
			keys.push_back(prevKeyPress);
		break;

		case SDLK_UP:
			if(jumpCount<2){
				tempJvel = jumpVel;
				jumpCount++;
			}
			cn=0;
			prevKeyPress = "UD";
			keys.push_back(prevKeyPress);			 
		break;

		case SDLK_DOWN:
			prevKeyPress = "DD";
			keys.push_back(prevKeyPress);		
		break;
		case SDLK_x:
			animate("blink");
			prevKeyPress = "XD";
			keys.push_back(prevKeyPress);
			// BYPASS = true;
		break;

		case SDLK_z:
			Bullet* bullet = new Bullet(collisionBox.x+collisionBox.w, collisionBox.y+10, "pbullet", flipType);
			object.push_back(bullet);
			BYPASS = true;
			animate("shoot");
			prevKeyPress = "ZD";
			keys.push_back(prevKeyPress);
			xDelPos = 0;
			cn=0;
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
		case SDLK_x:
			if(flipType == SDL_FLIP_HORIZONTAL){
				updatePos(collisionBox.x-200, collisionBox.y);
			}
			else{
				updatePos(collisionBox.x+200, collisionBox.y);
			}
		break;
		case SDLK_z:
			if(prevKeyPress == "RD"){
				xDelPos += (maxVel*TIME_STEP)*SCALE;
			}
			else if(prevKeyPress == "LD"){
				xDelPos -= (maxVel*TIME_STEP)*SCALE;
			}
		break;
		
	}
}

void Player::playerHitStatic(GameObj object){
	if(yPrevPos + collisionBox.h <= (object).collisionBox.y){
		collisionBox.y = (object).collisionBox.y - collisionBox.h;
		yDelPos = 0;
		tempJvel = 0;
		jumpCount = 0;
		isCollidingBelow = true;
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

//===========================OVERRIDEN updatePos()==================================//
void Player::updatePos(int x, int y){
	collisionBox.x = x;
	collisionBox.y = y;
}
//====================================END===========================================//


//========================UPDATE POSITION GENERAL===================================//
void Player::updatePos(){
	static string prev;
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

	//*********SETTING ANIMATION ACTIONS********//
	if(BYPASS){
		animate(prevAct);
	}
	else if(abs(xDelPos)>=EPSILON && abs(tempJvel)<=0.85){
		animate("run");
	}
	else{
		if(abs(xDelPos)<=EPSILON && abs(tempJvel)<=0.85){
			animate("stand");
		}
		else{

			animate("jump");
		}
	}
	isCollidingBelow = false;
}
//===================================END===================================================//

void Player::bulletHitPlayer(GameObj obj){
	health -= 1;
	animate("damage");
	cn=0;
	BYPASS = true;
}

void Player::animate(string act){
	// static int i=0;
	if(act == "run"){
		renderingClip = run[cn/5];
		cn++;
		if(cn==20){
			cn=0;
		}
		prevAct = "run";
	}
	else if(act == "shoot"){
		renderingClip = run[14+cn/4];
		cn++;
		if(cn>=8){
			cn=0;
			BYPASS = false;
		}
		prevAct = "shoot";
	}
	else if(act == "stand"){
		renderingClip = run[5];
		prevAct = "stand";
	}
	else if(act == "damage"){
		renderingClip = run[13+cn/10];
		cn++;
		if(cn==10){
			cn=0;
			BYPASS = false;
		}
		prevAct = "damage";
	}
	
	// else if(act == "blink"){
	// 	renderingClip = run[16+cn/1];
	// 	cn++;
	// 	if(cn==2){
	// 		cn=0;
	// 		BYPASS = false;
	// 	}
	// 	prevAct = "blink";		

	// }
	
	else if(act == "jump"){
		if(tempJvel>0 && abs(xDelPos)>=EPSILON){
			renderingClip = run[6];
		}
		else if(tempJvel>0 && abs(xDelPos)<=EPSILON){
			renderingClip = run[7];
		}
		else if(tempJvel < 0){
			renderingClip = run[9];
		}
	}
	
}



