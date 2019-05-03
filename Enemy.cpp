#include "Global.h"
#include "Bullet.h"
#include "Enemy.h"
#include <cmath>
#include <fstream>

using namespace std;

Enemy::Enemy(){}

Enemy::Enemy(float x, float y, int obj_code, std::string enemyname){
	initializeClips();
	name = enemyname;
	health = 100;
	isCollidingBelow = false;
	collisionBox.x = x;
	collisionBox.y = y;
	collisionBox.w = 75.0;
	collisionBox.h = 94.0;	
	code = obj_code;
	xPrevPos = x;
	yPrevPos = y;
	maxVel = 1.5;
	jumpVel = 4.0;
	tempJvel = 0.0;
	jumpCount = 0;
	xDelPos =0.0;
	yDelPos =0.0;
	//camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	renderingClip.x = run[0].x;
	renderingClip.y = run[0].y;
	renderingClip.w = run[0].w;
	renderingClip.h = run[0].h;
	flipType = SDL_FLIP_HORIZONTAL;

}


void Enemy::initializeClips(){
	ifstream in;
	string act;
	SDL_Rect box;
	in.open("EnemyClips.txt");
	while(!in.eof()){
		in>>act;
		in>> box.x >> box.y >> box.w >> box.h;
		if(act == "run"){
			run.push_back(box);
		}
	}
}

void Enemy::renderHealthBar(SDL_Rect fillHealth){
	float percentHealth = health/100.0;
	int barWidth = collisionBox.w-25;
	SDL_SetRenderDrawColor( gameRenderer, 0, 100, 0, 0xFF );
	SDL_Rect emptyHealth;
	if(flipType == SDL_FLIP_NONE){
		fillHealth.x = fillHealth.x+25;
	}
	else{
		fillHealth.x = fillHealth.x-10;
	}
	fillHealth.y = fillHealth.y - 10;
	fillHealth.h = 5;
	fillHealth.w = barWidth*percentHealth;
	SDL_RenderFillRect(gameRenderer, &fillHealth);
	emptyHealth.x = fillHealth.x+fillHealth.w;
	emptyHealth.y = fillHealth.y;
	emptyHealth.h = 5;
	emptyHealth.w = barWidth*(1.0-percentHealth);
	SDL_SetRenderDrawColor( gameRenderer, 255, 0, 0, 0xFF );
	SDL_RenderFillRect(gameRenderer, &emptyHealth);
}


void Enemy::render(float x, float y, SDL_Rect* clip, SDL_RendererFlip flipType){
	SDL_Rect spriteRect = {(int)x, (int)y, 0, 0};
	spriteRect.w = clip->w*1.5;
	spriteRect.h = clip->h*1.5;
	SDL_RenderCopyEx(gameRenderer, EnemySheetTexture, clip, &spriteRect, 0, NULL, flipType);
	Enemy::renderHealthBar(spriteRect);
}



void Enemy::updatePos(SDL_Rect playerCollisionBox){
	static int delay = 120;
	xPrevPos = collisionBox.x;
	yPrevPos = collisionBox.y;
	yDelPos = ((-1*tempJvel*TIME_STEP + (0.5)*GRAVITY*TIME_STEP*TIME_STEP)*SCALE);
	tempJvel = tempJvel - (GRAVITY*TIME_STEP);
	// cout<<playerCollisionBox.x - collisionBox.x<<"	";
	if((playerCollisionBox.x - collisionBox.x) > 500){
		xDelPos = 0;/*(maxVel*TIME_STEP)*SCALE;*/
		flipType = SDL_FLIP_NONE;
	}
	else{
		if((playerCollisionBox.x - collisionBox.x) < -500){
			xDelPos = 0/*-1*(maxVel*TIME_STEP)*SCALE*/;
			flipType = SDL_FLIP_HORIZONTAL;
		}
		if((playerCollisionBox.x - collisionBox.x) < 500 && (playerCollisionBox.x - collisionBox.x) > 200){
			xDelPos = (maxVel*TIME_STEP)*SCALE;
			flipType = SDL_FLIP_NONE;
		}
		if((playerCollisionBox.x - collisionBox.x) > -500 && (playerCollisionBox.x - collisionBox.x) < -200){
			xDelPos = -1*(maxVel*TIME_STEP)*SCALE;
			flipType = SDL_FLIP_HORIZONTAL;
		}
		if((playerCollisionBox.x - collisionBox.x) > -200 && (playerCollisionBox.x - collisionBox.x) < 200){
			xDelPos = 0;
			if(delay == 0){
				// cout<<delay<<" ";
				delay = 120;
				Bullet* bullet = new Bullet(collisionBox.x+collisionBox.w, collisionBox.y+40, "ebullet", flipType);
				object.push_back(bullet);
			}
			
		}
	}
	delay <= 0 ? 0 : delay--;
	collisionBox.x += xDelPos;
	collisionBox.y += yDelPos;
	// cout<<collisionBox.y<<"		"<<yPrevPos<<endl;
	if(abs(xDelPos)>=EPSILON && isCollidingBelow==true){
		animateRun(true);
	}
	else{
		animateRun(false);
	}
	// isCollidingBelow = false;
}

void Enemy::enemyHitStatic(GameObj object){
	// cout<<"		"<<yPrevPos<<"	"<<collisionBox.h<<"	"<<(object).collisionBox.y<<endl;
	if(yPrevPos + collisionBox.h <= (object).collisionBox.y){
		collisionBox.y = (object).collisionBox.y - collisionBox.h;
		yDelPos = 0;
		tempJvel = 0.0;
		jumpCount = 0;
		isCollidingBelow = true;
		// cout<<"b";
	}
	else{
		if(xPrevPos + collisionBox.w <= (object).collisionBox.x){
			collisionBox.x = (object).collisionBox.x - collisionBox.w;
			// cout<<"a"<<endl;
		}
		else{
			if(xPrevPos >= (object).collisionBox.x + (object).collisionBox.w){
				collisionBox.x = (object).collisionBox.x + (object).collisionBox.w;
				// cout<<"c"<<endl;
			}
			else{
				if(yPrevPos >= (object).collisionBox.y + (object).collisionBox.h){
					collisionBox.y = (object).collisionBox.y + (object).collisionBox.h;
					tempJvel = 0;
					// cout<<"d"<<endl;	
				}
			}
		}
	}
}

void Enemy::bulletHitEnemy(GameObj obj){
	health -= 5;
}


void Enemy::animateRun(bool anim){
	static int i=0;
	renderingClip = run[i/6];
	//cout<<i<<endl;
	i++;
	if(i==36 || anim==false){
		i=0;
	}
}
