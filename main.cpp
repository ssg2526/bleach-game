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
const float FREQUENCY = 60.0;
const float TIME_STEP = 1.0/FREQUENCY;
const int SCREEN_WIDTH = 1200;
const int LEVEL_WIDTH = 2880;
const int LEVEL_HEIGHT = 700;
const int SCREEN_HEIGHT = 700;
const float SECOND = 1000000.0;
const float EPSILON = .1;
const float SCALE = 100.0;
bool BYPASS = false;

SDL_Window* gWindow = NULL;
SDL_Texture* EnemySheetTexture = NULL;
SDL_Texture* PlayerSheetTexture = NULL;
SDL_Texture* TileSheetTexture = NULL;
SDL_Texture* MoonTexture = NULL;

SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};


bool init();

bool loadMedia();

void close();
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
				SDL_SetRenderDrawColor(gameRenderer, 0, 0, 0, 255);
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

	//Load sprite sheet textureTIME_STEP
	if(!loadFromFile("sprites_folder/ichigo3.png", "player")){
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

	//Destroy window
	// SDL_DestroyTexture(player->PlayerSheetTexture);	
	SDL_DestroyRenderer( gameRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gameRenderer = NULL;

	//Quit SDL subsystems
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


void render_world(SDL_Rect playerBox){
	// SDL_SetRenderDrawColor( gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_SetRenderDrawColor( gameRenderer, 0, 0, 0, 255 );
	SDL_RenderClear( gameRenderer );
	SDL_SetRenderDrawColor( gameRenderer, 255, 255, 255, 255 );	
	// SDL_SetRenderDrawColor( gameRenderer, 0x00, 0x00, 0x00, 0xFF );
	camera.x = (playerBox.x+playerBox.w/2)-SCREEN_WIDTH*3.0/5;
	camera.y = 0;
	if( camera.x < 0 )
	{ 
		camera.x = 0;
	}
	if(camera.x + SCREEN_WIDTH > LEVEL_WIDTH){
		camera.x = LEVEL_WIDTH - SCREEN_WIDTH;
	}
	// for(int i=0; i<1000; i++){

	// }
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

void update_world(SDL_Rect playerBox){
	for(int i=0; i<object.size(); i++){
		if(object[i]->name != "ground"){
			if(object[i]->name == "e1"){
				Enemy* e = dynamic_cast<Enemy*>(object[i]);
				if(e->health <= 0){
					delete object[i];
					object.erase(object.begin()+i);
					continue;
				}
				object[i]->updatePos(playerBox);
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

int main(int argc, char* args[]){
	
	Player* player = new Player(200, 400, 1, "player");
	Enemy* e1 = new Enemy(950 ,250, -1, "e1");
	CollisionDetector c_detector;// = new CollisionDetector();
	initiate_tiles();
	object.push_back(e1);
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
			
			while(!quit){
				while(SDL_PollEvent(&e)!=0){
					
					if(e.type == SDL_QUIT){
						quit = true;
					}
					else{
						if(!BYPASS){
							if(e.type == SDL_KEYDOWN && e.key.repeat==0){
								player->handleMovement(e);
							}
							else{
								if(e.type == SDL_KEYUP){
									player->handleMovement(e,1);
								}
							}
						}
						
					}
				}

				update_world(player->collisionBox);

				c_detector.checkCollision(object);

				render_world(player->collisionBox);


				SDL_RenderPresent( gameRenderer );
				usleep(SECOND*TIME_STEP);
				
			}
		}
	}
	close(player);
	closee(e1);
	return 0;
}


