#ifndef TILE_H
#define TILE_H
#include "Global.h"
#include "GameObj.h"
#include <iostream>

class Tile: public GameObj{
    public:
        Tile(int x, int y, std::string type);
        // SDL_Rect collisionBox;
        string type;
        void render(float camx, float camy, SDL_Rect* clip = NULL, SDL_RendererFlip flipType=SDL_FLIP_NONE);
        void render_tile_set(vector<Tile> tile_set);

};

#endif
