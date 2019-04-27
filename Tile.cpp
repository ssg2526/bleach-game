#include "Global.h"
#include "Tile.h"
#include <cmath>
#include <fstream>

using namespace std;
Tile::Tile(int x, int y, string tile_type){
    collisionBox.x = x;
    collisionBox.y = y;
    collisionBox.w = 60;
    collisionBox.h = 50;
    type = tile_type;
}

void Tile::render(float x, float y, SDL_Rect* clip, SDL_RendererFlip flipType){
    SDL_Rect spriteRect = {(int)x, (int)y, 0, 0};
	spriteRect.w = clip->w;
	spriteRect.h = clip->h;
    SDL_RenderCopyEx(gameRenderer, TileSheetTexture, clip, &spriteRect, 0, NULL, flipType);
	// cout<<"s";
}