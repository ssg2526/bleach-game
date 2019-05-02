#include "Global.h"
#include "Tile.h"
#include <cmath>
#include <fstream>

using namespace std;

Tile::Tile(){}

Tile::Tile(float x, float y, int obj_code, string tile_type, string obj_name){
    collisionBox.x = x;
    collisionBox.y = y;
    collisionBox.w = 60.0;
    collisionBox.h = 50.0;
    type = tile_type;
	name = obj_name;
    code = obj_code;
    flipType = SDL_FLIP_NONE;
    renderingClip = {772, 654, 60, 50};
}


void Tile::render(float x, float y, SDL_Rect* clip, SDL_RendererFlip flipType){
    SDL_Rect spriteRect = {(int)x, (int)y, 0, 0};
	spriteRect.w = clip->w;
	spriteRect.h = clip->h;
    SDL_RenderCopyEx(gameRenderer, TileSheetTexture, clip, &spriteRect, 0, NULL, flipType);
	// cout<<"s";
}
