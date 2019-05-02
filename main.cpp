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


bool init();

bool loadMedia();

void close();

vector<GameObj*> object;
// vector<Tile> tile_obj;

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
	return false;
	
}


bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load sprite sheet texture
	if(!loadFromFile("sprites_folder/ichigo3.png", "player")){
		success = false;
	}
	else if(!loadFromFile("sprites_folder/enemy1.png", "enemy")){
		success = false;
	}
	else if(!loadFromFile("sprites_folder/tiles.png", "tiles")){
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


void closee(Enemy* player)
{
	//Free loaded images
	(*player).free();
}

void closeTile(Tile* tile){
	(*tile).free();
}

void render_tile_set(vector<Tile> &tile_obj, SDL_Rect cam){
    SDL_Rect clip = {772, 654, 60, 50};
    for(int i=0; i<tile_obj.size(); i++){
		tile_obj[i].render(tile_obj[i].collisionBox.x-cam.x, tile_obj[i].collisionBox.y-cam.y, &clip, SDL_FLIP_NONE);
        
    }
}

void tiles_to_object_list(vector<Tile> &tile_obj){
	for(int i=0; i<tile_obj.size(); i++){
		object.push_back(&tile_obj[i]);
		// cout<<(*object[i]).name<<" ";
	}
}

void initiate_tiles(vector<Tile> &tile_obj){
	ifstream in;
	string type;
	in.open("tile_map.txt");
	float x=0,y=0;
	int i=1,ct=0;
	while(!in.eof()){
		in>>type;
		if(type == "1"){
			// Tile tile(x, y, 0, type, "ground");
			Tile* tile = new Tile(x, y, 0, type, "ground");
			tile_obj.push_back(*tile);
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

void update_bullets(vector<Bullet> &bullet_vec){
	for(int i=0; i<bullet_vec.size(); i++){
		bullet_vec[i].updatePos(); 
	}
}

void bullet_to_object_list(vector<Bullet> &bullet_vec){
	for(int i=0; i<bullet_vec.size(); i++){
		object.push_back(&bullet_vec[i]);
		// cout<<(*object[i]).name<<" ";
	}
}

void render_bullets(vector<Bullet> &bullet_vec, SDL_Rect camera){
	for(int i=0; i<bullet_vec.size(); i++){
		// bullet_vec[i].render(); 
		SDL_Rect blt = {bullet_vec[i].collisionBox.x-camera.x, bullet_vec[i].collisionBox.y, bullet_vec[i].collisionBox.w, bullet_vec[i].collisionBox.h};
		SDL_RenderDrawRect( gameRenderer, &blt );
	}
}

void render_world(int camx, int camy){
	SDL_SetRenderDrawColor( gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( gameRenderer );
	SDL_SetRenderDrawColor( gameRenderer, 0x00, 0x00, 0x00, 0xFF );

	for(int i=0; i<object.size(); i++){
		if(object[i]->name == "pbullet"){
			SDL_Rect blt = {object[i]->collisionBox.x-camx, object[i]->collisionBox.y, object[i]->collisionBox.w, object[i]->collisionBox.h};
			SDL_RenderDrawRect( gameRenderer, &blt );
		}
		else{
			(*object[i]).render(object[i]->collisionBox.x-camx, object[i]->collisionBox.y, &object[i]->renderingClip, object[i]->flipType);
		}
		
	}
}

int main(int argc, char* args[]){
	
	vector<Tile> tile_obj;
	vector<Bullet> bullet_vec;
	Player* player = new Player(200, 300, 1, "player");
	Enemy* e1 = new Enemy(750 ,250, -1, "e1");
	CollisionDetector c_detector;// = new CollisionDetector();
	initiate_tiles(tile_obj);
	object.push_back(e1);
	object.push_back(player);
	// Bullet* bullet;
	Bullet* bullet = new Bullet(player->collisionBox.x+player->collisionBox.w, player->collisionBox.y+10, "pbullet");
	object.push_back(bullet);
	
	
	bool stopAnimation = true;
	SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
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
								if(e.key.keysym.sym == SDLK_z){
									// (*bullet).~Bullet();
									delete bullet;
									// cout<<"zxcv";
								}
							}
							else{
								if(e.type == SDL_KEYUP){
									player->handleMovement(e,1);
								}
							}
						}
						
					}
				}
				player->updatePos();
				e1->updatePos(player->collisionBox);
				// bullet.updatePos();
				// bullet_to_object_list(bullet_vec);
				// update_bullets(bullet_vec);
				bullet->updatePos();
				// cout<<bullet->collisionBox.x<<" ";
				
				c_detector.checkCollision(object);
				
				/////////////////////////////RENDERING////////////////////////////////

				
				camera.x = (player->collisionBox.x+player->collisionBox.w/2)-SCREEN_WIDTH/2;
				camera.y = 0;
				if( camera.x < 0 )
				{ 
					camera.x = 0;
				}
				if(camera.x + SCREEN_WIDTH > LEVEL_WIDTH){
					camera.x = LEVEL_WIDTH - SCREEN_WIDTH;
				}
				render_world(camera.x, camera.y);
				

				SDL_RenderPresent( gameRenderer );
				usleep(SECOND*TIME_STEP);
				
			}
		}
	}
	close(player);
	closee(e1);
	for(int i=0;i<tile_obj.size();i++){
		// close(&tile_obj[i]);
		// Tile* p = &tile_obj[i]; 
		// delete &tile_obj[i];
		// tile_obj[i]=NULL;
		closeTile(&tile_obj[i]);
	}
	return 0;
}


