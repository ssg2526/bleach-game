#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unistd.h>
#include "Global.h"
#include "Player.h"
#include "GameObj.h"
#include "Enemy.h"
#include "Tile.h"
#include "Bullet.h"
#include "CollisionDetector.h"
using namespace std;
const double FREQUENCY = 60.0;
const double TIME_STEP = 1.0/FREQUENCY;
const int SCREEN_WIDTH = 1200;
const int LEVEL_WIDTH = 2880;
const int LEVEL_HEIGHT = 700;
const int SCREEN_HEIGHT = 700;
const double SECOND = 1000000.0;
const double EPSILON = 0.1;
const double SCALE = 100.0;
bool BYPASS = false;
bool PAUSE = false;

Mix_Music *gMusic = NULL; //The sound effects that will be used 
Mix_Chunk *gScratch = NULL; 
Mix_Chunk *gHigh = NULL; 
Mix_Chunk *gMedium = NULL; 
Mix_Chunk *gLow = NULL;

SDL_Window* gWindow = NULL;
SDL_Texture* EnemySheetTexture = NULL;
SDL_Texture* PlayerSheetTexture = NULL;
SDL_Texture* TileSheetTexture = NULL;
SDL_Texture* MoonTexture = NULL;
SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
SDL_Rect menu_screen = {50, 50, SCREEN_WIDTH/4, SCREEN_HEIGHT/4};
// SDL_RenderSetViewport(gameRenderer, &menu_screen );
bool init();

bool loadMedia();

void close();
vector<GameObj*> object;

bool init(){
	bool success = true;
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)<0){
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
			// SDL_RenderSetViewport(gameRenderer, &menu_screen );
			

			if(gameRenderer == NULL){
				cout<<"renderer could not be created";
			}
			else{
				SDL_SetRenderDrawColor(gameRenderer, 0, 0, 0, 255);
				int imgFlags = IMG_INIT_PNG;
				if(!(IMG_Init(imgFlags) & imgFlags)){
					cout<<"SDL image could not be initialized";
					success = false;
				}

				if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ){ 
					printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() ); success = false; 
				}
			}
		}
	}
	return success;
}

bool loadFromFile(string path, string name){
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );
	newTexture = SDL_CreateTextureFromSurface(gameRenderer, loadedSurface);
	SDL_FreeSurface(loadedSurface);

	


	if(name == "enemy"){
		EnemySheetTexture = newTexture;
		newTexture = NULL;
		return EnemySheetTexture!=NULL;
	}
	else if(name == "player"){
		PlayerSheetTexture = newTexture;
		newTexture = NULL;
		return PlayerSheetTexture!=NULL;
	}
	else if(name == "tiles"){
		TileSheetTexture = newTexture;
		newTexture = NULL;
		return TileSheetTexture!=NULL;
	}
	else if(name == "moon"){
		MoonTexture = newTexture;
		newTexture = NULL;
		return MoonTexture!=NULL;
	}
	return false;
	
}


bool loadMedia()
{
	//Loading success flag
	bool success = true;

	gMusic = Mix_LoadMUS( "bleach.mp3" ); 
	if( gMusic == NULL ) { 
		printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() ); 
		// success = false
		success = false; 
	}

	gHigh = Mix_LoadWAV( "high.wav" ); 
	if( gHigh == NULL ) {
		printf( "Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError() ); 
		success = false; 
	}

	//Load sprite sheet textureTIME_STEP
	if(!loadFromFile("sprites_folder/ichigo4.png", "player")){
		success = false;
	}
	if(!loadFromFile("sprites_folder/enemy1.png", "enemy")){
		success = false;
	}
	if(!loadFromFile("sprites_folder/tiles.png", "tiles")){
		success = false;
	}
	if(!loadFromFile("sprites_folder/moon3.png", "moon")){
		success = false;
	}
	return success;
}


void close(Player* player)
{
	//Free loaded images

	Mix_FreeChunk( gHigh );
	gHigh = NULL;
	//Free the music 
	Mix_FreeMusic( gMusic ); 
	gMusic = NULL;
	//Destroy window
	// SDL_DestroyTexture(player->PlayerSheetTexture);	
	SDL_DestroyRenderer( gameRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gameRenderer = NULL;

	//Quit SDL subsystems
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}


void closee(Enemy* player)
{
	//Free loaded images
	// (*player).free();
}

void closeTile(Tile* tile){
	// (*tile).free();
}


void initiate_tiles(){
	ifstream in;
	string type;
	in.open("tile_map.txt");
	float x=0,y=0;
	int i=1,ct=0;
	while(!in.eof()){
		in>>type;
		if(type == "1"){
			Tile* tile = new Tile(x, y, 0, type, "ground");
			tile->renderingClip = {772, 654, 128, 128};
			object.push_back(tile);

		}
		else if(type == "2"){
			Tile* tile = new Tile(x, y, 0, type, "ground");
			tile->renderingClip = {514, 400, 128, 128};
			object.push_back(tile);
		}
		x = x+60;
		if(i%48 == 0){
			x = 0;
			y = y+50;
		}
		i++;
	}
}

void updateCam(Player* player){
	int frontDiff = (player->collisionBox.x+player->collisionBox.w/2) - SCREEN_WIDTH*3.0/5;
	int backDiff = (player->collisionBox.x+player->collisionBox.w/2) - SCREEN_WIDTH*1.5/5;
	if(frontDiff - camera.x >= 0){
		if(frontDiff - camera.x < 10){
			camera.x = frontDiff;
		}
		else{
			camera.x += player->maxVel*4;
		}
	}
	if(backDiff - camera.x <= 0){
		if(backDiff - camera.x > -10){
			camera.x = backDiff;
		}
		else{
			camera.x -= player->maxVel*4;
		}
	}
	camera.y = 0;
	if( camera.x < 0 )
	{ 
		camera.x = 0;
	}
	if(camera.x + SCREEN_WIDTH > LEVEL_WIDTH){
		camera.x = LEVEL_WIDTH - SCREEN_WIDTH;
	}
}

void render_world(Player* player){
	// SDL_SetRenderDrawColor( gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_SetRenderDrawColor( gameRenderer, 0, 0, 0, 255 );
	SDL_RenderClear( gameRenderer );
	SDL_SetRenderDrawColor( gameRenderer, 255, 255, 255, 255 );	
	// SDL_SetRenderDrawColor( gameRenderer, 0x00, 0x00, 0x00, 0xFF );
	updateCam(player);
	SDL_Rect rect= {900, 50, 120, 120};
	SDL_Rect clip = {336, 351, 400, 400};
	SDL_RenderCopy(gameRenderer, MoonTexture, &clip, &rect);
	for(int i=0; i<object.size(); i++){
		if(object[i]->name == "pbullet" || object[i]->name == "ebullet"){
			SDL_Rect blt = {object[i]->collisionBox.x-camera.x, object[i]->collisionBox.y, object[i]->collisionBox.w, object[i]->collisionBox.h};
			SDL_RenderDrawRect( gameRenderer, &blt );
		}
		else{
			(*object[i]).render(object[i]->collisionBox.x-camera.x, object[i]->collisionBox.y, &object[i]->renderingClip, object[i]->flipType);
		}
		SDL_SetRenderDrawColor(gameRenderer, 255, 255, 255, 255);
	}
}

void update_world(Player* player){
	for(int i=0; i<object.size(); i++){
		if(object[i]->name != "ground"){
			if(object[i]->name == "e1"){
				Enemy* e = dynamic_cast<Enemy*>(object[i]);
				if(e->health <= 0){
					delete object[i];
					object.erase(object.begin()+i);
					continue;
				}
				object[i]->updatePos(player->collisionBox);
			}
			else{
				if(object[i]->name == "pbullet"){
					Bullet* b = dynamic_cast<Bullet*>(object[i]);
					if(abs(b->startPos - b->collisionBox.x) >= b->lifeSpan){
						delete object[i];
						object.erase(object.begin()+i);
						continue;
					}
				}
				object[i]->updatePos();
				
			}
		}
	}
}

// void setRenderingClip(){
// 	for(int i=0; i<object.size(); i++){
// 		object[i]->renderingClip();
// 	}
// }

int main(int argc, char* args[]){
	
	Player* player = new Player(200, 400, 1, "player");
	Enemy* e1 = new Enemy(1000 ,250, -1, "e1");
	Enemy* e2 = new Enemy(800 ,250, -1, "e1");
	CollisionDetector c_detector;
	initiate_tiles();
	object.push_back(e1);
	object.push_back(e2);
	object.push_back(player);
	
	bool stopAnimation = true;
	int i=0;
	if(!init()){
		cout<<"failed to initialize";
	}
	else{
		if(!loadMedia()){
			cout<<"unable to load media";
		}
		else{
			bool quit = false;
			SDL_Event e;
			int ct=0;
			Mix_PlayMusic( gMusic, -1 );			
			while(!quit){
				while(SDL_PollEvent(&e)!=0){					
					if(e.type == SDL_QUIT){
						quit = true;
					}
					else{
						if(e.type == SDL_KEYDOWN && e.key.repeat==0){
							if(!PAUSE && !BYPASS){
								player->handleMovement(e);
							}

							if(e.key.keysym.sym == SDLK_1){
								Mix_PlayChannel(-1, gHigh, 0);
							}
							if(e.key.keysym.sym == SDLK_9){
								Mix_PlayMusic( gMusic, -1 );
							}
							if(e.key.keysym.sym == SDLK_ESCAPE){
								if(PAUSE){
									PAUSE = false;
								}
								else{
									PAUSE = true;
									// SDL_RenderCopy( gameRenderer, PlayerSheetTexture, NULL, NULL);
								}
							}
						}
						else{
							if(e.type == SDL_KEYUP){
								if(!PAUSE && !BYPASS){
									player->handleMovement(e,1);
								}
							}
						}
					}
				}

				if(!PAUSE){
					update_world(player);
					c_detector.checkCollision(object);
				}
				

				// setRenderingClip();

				render_world(player);

				SDL_RenderPresent( gameRenderer );
				usleep(SECOND*TIME_STEP);
				
			}
		}
	}
	close(player);
	closee(e1);
	return 0;
}


