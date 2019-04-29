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
	if(!loadFromFile("sprites_folder/sprites.png", "player")){
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
			Tile tile(x, y, 0, type, "ground");
			tile_obj.push_back(tile);
			// object.push_back(&tile_obj[i-1]);

		}
		x = x+60;
		if(i%48 == 0){
			x = 0;
			y = y+50;
		}
		i++;
	}
}

int main(int argc, char* args[]){
	
	vector<Tile> tile_obj;
	Player player(200, 200, 1, "player");
	Enemy e1(550 ,200, -1, "e1");
	CollisionDetector c_detector;// = new CollisionDetector();
	GameObj ground(-10, 300, 3000, 50, 0, "game_object");
	GameObj wall(452, 140, 60, 150, 0, "game_object");
	initiate_tiles(tile_obj);
	tiles_to_object_list(tile_obj);
	object.push_back(&ground);
	object.push_back(&wall);
	object.push_back(&e1);
	object.push_back(&player);
	
	
	
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
				camera.x = (player.collisionBox.x+player.collisionBox.w/2)-SCREEN_WIDTH/2;
				camera.y = 0;
				if( camera.x < 0 )
				{ 
					camera.x = 0;
				}
				if(camera.x + SCREEN_WIDTH > LEVEL_WIDTH){
					camera.x = LEVEL_WIDTH - SCREEN_WIDTH;
				}
				render_tile_set(tile_obj, camera);
				player.render(player.collisionBox.x - camera.x, player.collisionBox.y - camera.y, &player.renderingClip, player.flipType);
				
				e1.render(e1.collisionBox.x - camera.x, e1.collisionBox.y - camera.y, &e1.renderingClip, e1.flipType);
				
				SDL_SetRenderDrawColor( gameRenderer, 0x00, 0x00, 0x00, 0xFF );

				SDL_Rect grnd = {ground.collisionBox.x-camera.x, ground.collisionBox.y, ground.collisionBox.w, ground.collisionBox.h};
				SDL_RenderDrawRect( gameRenderer, &grnd );

				SDL_Rect wll = {wall.collisionBox.x-camera.x, wall.collisionBox.y, wall.collisionBox.w, wall.collisionBox.h};

				SDL_RenderDrawRect( gameRenderer, &wll );
				SDL_RenderPresent( gameRenderer );
				usleep(SECOND*TIME_STEP);
				
			}
		}
	}
	close(&player);
	closee(&e1);
	for(int i=0;i<tile_obj.size();i++){
		// close(&tile_obj[i]);
		// Tile* p = &tile_obj[i]; 
		// delete &tile_obj[i];
		// tile_obj[i]=NULL;
		closeTile(&tile_obj[i]);
	}
	return 0;
}


