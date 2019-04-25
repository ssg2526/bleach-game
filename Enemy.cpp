#include "Global.h"
#include "Enemy.h"
#include <cmath>
#include <fstream>

using namespace std;

Enemy::Enemy(){}

Enemy::Enemy(float x, float y, std::string enemyname){
	initializeClips();
	loadEnemyFromFile("sprites_folder/enemy.png");
	EnemySheetTexture = NULL;
	EnemySheetHeight = 0;
	EnemySheetWidth = 0;
	name = enemyname;
	isCollidingBelow = false;
	collisionBox.x = x;
	collisionBox.y = y;
	collisionBox.w = 50.0;
	collisionBox.h = 66.0;	
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

Enemy::~Enemy(){
	free();
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

bool Enemy::loadEnemyFromFile(string path){
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );
	newTexture = SDL_CreateTextureFromSurface(gameRenderer, loadedSurface);
	SDL_FreeSurface(loadedSurface);
	EnemySheetTexture = newTexture;
	EnemySheetWidth = loadedSurface->w;
	EnemySheetHeight = loadedSurface->h;
	newTexture = NULL;
	//return PlayerSheetTexture!=NULL;
	return EnemySheetTexture!=NULL;
}

void Enemy::render(float x, float y, SDL_Rect* clip, SDL_RendererFlip flipType){
	SDL_Rect spriteRect = {x, y, EnemySheetWidth, EnemySheetHeight};
	spriteRect.w = clip->w;
	spriteRect.h = clip->h;
	SDL_RenderCopyEx(gameRenderer, EnemySheetTexture, clip, &spriteRect, 0, NULL, flipType);
}



void Enemy::updatePos(SDL_Rect playerCollisionBox){
	xPrevPos = collisionBox.x;
	yPrevPos = collisionBox.y;
	yDelPos = ((-1*tempJvel*TIME_STEP + (0.5)*GRAVITY*TIME_STEP*TIME_STEP)*SCALE);
	tempJvel = tempJvel - (GRAVITY*TIME_STEP);
	// cout<<playerCollisionBox.x - collisionBox.x<<"	";
	if((playerCollisionBox.x - collisionBox.x) > 300){
		xDelPos = 0;/*(maxVel*TIME_STEP)*SCALE;*/
		flipType = SDL_FLIP_NONE;
	}
	else{
		if((playerCollisionBox.x - collisionBox.x) < -300){
			xDelPos = 0/*-1*(maxVel*TIME_STEP)*SCALE*/;
			flipType = SDL_FLIP_HORIZONTAL;
		}
		if((playerCollisionBox.x - collisionBox.x) < 300 && (playerCollisionBox.x - collisionBox.x) > 50){
			xDelPos = (maxVel*TIME_STEP)*SCALE;
			flipType = SDL_FLIP_NONE;
		}
		if((playerCollisionBox.x - collisionBox.x) > -300 && (playerCollisionBox.x - collisionBox.x) < -50){
			xDelPos = -1*(maxVel*TIME_STEP)*SCALE;
			flipType = SDL_FLIP_HORIZONTAL;
		}
		if((playerCollisionBox.x - collisionBox.x) > -50 && (playerCollisionBox.x - collisionBox.x) < 50){
			xDelPos = 0;
		}
	}
	
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


void Enemy::animateRun(bool anim){
	static int i=0;
	renderingClip = run[i/6];
	//cout<<i<<endl;
	i++;
	if(i==36 || anim==false){
		i=0;
	}
}

void Enemy::free(){
	SDL_DestroyTexture(EnemySheetTexture);
	EnemySheetTexture = NULL;
	EnemySheetHeight = 0;
	EnemySheetWidth = 0;
}