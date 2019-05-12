#include "GameObj.h"
#include "Global.h"
#include "Bullet.h"
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
		// if(act == "run"){
		// 	run.push_back(box);
		// }
		// if(act=="stand"){
		// 	run.push_back(box);
		// }
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
		case SDLK_x:
			xDelPos = 0;
			animate("blink");
			// BYPASS = true;
			BYPASS = false;
			// animate("shoot");
			// Bullet bullet(collisionBox.x+collisionBox.w, collisionBox.y+collisionBox.h/4);
		break;

		case SDLK_z:
			xDelPos = 0;
			Bullet* bullet = new Bullet(collisionBox.x+collisionBox.w, collisionBox.y+10, "pbullet", flipType);
			object.push_back(bullet);
			BYPASS = false;
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
	static string prev;
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
			// prev = "shoot";
			// animate("damage");

	}
	else if(abs(xDelPos)>=EPSILON && isCollidingBelow==true){
		animate("run");
	}
	
	else{
		// if(isCollidingBelow == true){
			// if(!)
			animate("stand");
		// }
		// else{
		// 	animate("jump");
		// }
	}
	// isCollidingBelow = false;
}

void Player::updatePos(int x, int y){
	collisionBox.x = x;
	collisionBox.y = y;
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

void Player::bulletHitPlayer(GameObj obj){
	health -= 1;
	animate("damage");
	BYPASS = true;
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
		renderingClip = run[12+i/4];
		i++;
		if(i>=12){
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
	else if(act == "damage"){
		// i = 0;
		renderingClip = run[15+i/4];
		i++;
		if(i==4){
			i=0;
			BYPASS = false;
		}
	}
	
	else if(act == "blink"){
		// i=0;
		renderingClip = run[17+i/10];
		i++;
		if(i==20){
			i=0;
		}
		if(flipType == SDL_FLIP_HORIZONTAL){
			updatePos(collisionBox.x-200, collisionBox.y);
		}
		else
		{
			updatePos(collisionBox.x+200, collisionBox.y);
		}
		

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



