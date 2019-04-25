#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include "Global.h"
#include "Player.h"
#include "GameObj.h"
#include "Enemy.h"
#include "CollisionDetector.h"
using namespace std;
const float FREQUENCY = 60.0;
const float TIME_STEP = 1.0/FREQUENCY;
const int SCREEN_WIDTH = 720;
const int SCREEN_HEIGHT = 817;
const float SECOND = 1000000.0;
const float EPSILON = .1;
const float SCALE = 100.0;

SDL_Window* gWindow = NULL;

bool init();

bool loadMedia();

void close();


SDL_Rect run[4];
SDL_Rect shoot[3];
SDL_Rect jump[4];
SDL_Rect spin[4];

vector<GameObj*> object;

bool init(){
	bool success = true;
	if(SDL_Init(SDL_INIT_VIDEO)<0){
		cout<<"couldn't initialize SDL";
		success = false;
	}
	else{
		if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")){
			cout<<"Warning: linear texture filtering not enabled";
		}
		
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if(gWindow == NULL){
			cout<<"Window could not be created!";
			success = false;
		}
		else{
			gameRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if(gameRenderer == NULL){
				cout<<"renderer could not be created";
			}
			else{
				SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				int imgFlags = IMG_INIT_PNG;
				if(!(IMG_Init(imgFlags) & imgFlags)){
					cout<<"SDL image could not be initialized";
					success = false;
				}
			}
		}
	}
	return success;
}


bool loadMedia(Player* player)
{
	//Loading success flag
	bool success = true;

	//Load sprite sheet texture
	if( !(*player).loadPlayerFromFile("sprites_folder/sprites.png"))
	{
		printf( "Failed to load sprite sheet texture!\n" );
		success = false;
	}
	else
	{
		run[0].x = 21;
		run[0].y = 308;
		run[0].w = 31;
		run[0].h = 43;

		run[1].x = 54;
		run[1].y = 308;
		run[1].w = 31;
		run[1].h = 43;
		
		run[2].x = 86;
		run[2].y = 308;
		run[2].w = 31;
		run[2].h = 43;

		run[3].x = 54;
		run[3].y = 308;
		run[3].w = 31;
		run[3].h = 43;

		shoot[0].x = 347;
		shoot[0].y = 652;
		shoot[0].w = 19;
		shoot[0].h = 22;


		shoot[1].x = 370;
		shoot[1].y = 651;
		shoot[1].w = 44;
		shoot[1].h = 37;

		shoot[2].x = 422;
		shoot[2].y = 658;
		shoot[2].w = 44;
		shoot[2].h = 23;

		spin[0].x =  20;
		spin[0].y = 484;
		spin[0].w = 36;
		spin[0].h = 44;

		spin[1].x = 59;
		spin[1].y = 485;
		spin[1].w = 40;
		spin[1].h = 43;

		spin[2].x = 103;
		spin[2].y = 482;
		spin[2].w = 38;
		spin[2].h = 46;

		spin[3].x = 146;
		spin[3].y = 490;
		spin[3].w = 39;
		spin[3].h = 38;

	}
	return success;
}


bool loadEnemy(Enemy* e1)
{
	//Loading success flag
	bool success = true;

	//Load sprite sheet texture
	if(!(*e1).loadEnemyFromFile("sprites_folder/enemy1.png"))
	{
		printf( "Failed to load enemy sprite sheet texture!\n" );
		success = false;
	}
	return success;
}

void close(Player* player)
{
	//Free loaded images
	(*player).free();

	//Destroy window	
	SDL_DestroyRenderer( gameRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gameRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* args[]){
	Player player(200, 400, "player");
	Enemy e1(550 ,400, "e1");
	CollisionDetector c_detector;// = new CollisionDetector();
	GameObj ground(-10, 500, 750, 50, "game_object");
	GameObj wall(452, 203, 60, 200, "game_object");
	object.push_back(&e1);
	object.push_back(&player);
	
	object.push_back(&ground);
	object.push_back(&wall);
	
	bool stopAnimation = true;
	SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	int i=0;
	if(!init()){
		cout<<"failed to initialize";
	}
	else{
		if(!loadMedia(&player)){
			cout<<"unable to load media";
		}
		else if(!loadEnemy(&e1)){
			cout<<"unable to load enemy";
		}
		else{
			bool quit = false;
			SDL_Event e;
			
			while(!quit){
				while(SDL_PollEvent(&e)!=0){
					if(e.type == SDL_QUIT){
						quit = true;
					}
					else{
						if(e.type == SDL_KEYDOWN && e.key.repeat==0){
							player.handleMovement(e);
						}
						else{
							if(e.type == SDL_KEYUP){
								player.handleMovement(e,1);
							}
						}
					}
				}
				e1.updatePos(player.collisionBox);
				player.updatePos();
				
				c_detector.checkCollision(object);

				SDL_SetRenderDrawColor( gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gameRenderer );
				//player.render(player.collisionBox.x, player.collisionBox.y, &run[i/4], player.flipType);
				camera.x = (player.collisionBox.x+player.collisionBox.w/2)-SCREEN_WIDTH/2;
				camera.y = (player.collisionBox.y+player.collisionBox.h/2)-SCREEN_HEIGHT/2;
				if( camera.x < 0 )
				{ 
					camera.x = 0;
				}
				if( camera.y < 0 )
				{
					camera.y = 0;
				}
				//player.render(0, 0, &camera);
				player.render(player.collisionBox.x, player.collisionBox.y, &player.renderingClip, player.flipType);
				e1.render(e1.collisionBox.x, e1.collisionBox.y, &e1.renderingClip, e1.flipType);
				
				SDL_SetRenderDrawColor( gameRenderer, 0x00, 0x00, 0x00, 0xFF );
				//ground.collisionBox.x = (SCREEN_WIDTH/2-camera.x);
				SDL_RenderDrawRect( gameRenderer, &ground.collisionBox );

				//SDL_SetRenderDrawColor( gameRenderer, 0x00, 0x00, 0x00, 0xFF );
				//wall.collisionBox.x = (SCREEN_WIDTH/2-camera.x);
				SDL_RenderDrawRect( gameRenderer, &wall.collisionBox );
				SDL_RenderDrawRect( gameRenderer, &e1.collisionBox);
				SDL_RenderDrawRect( gameRenderer, &player.collisionBox);
				SDL_RenderPresent( gameRenderer );
				usleep(SECOND*TIME_STEP);
			}
		}
	}
	close(&player);
	// close(&e1);
	return 0;
}


