#ifndef TILE_H
#define TILE_H
#include "Global.h"
#include "GameObj.h"
#include <iostream>

class Tile: public GameObj{
    public:
        Tile();
        Tile(float x, float y, int obj_code,std::string type, std::string obj_name="notile");
        // SDL_Rect collisionBox;
        ~Tile();
        string type;
        // int code;
        void render(float camx, float camy, SDL_Rect* clip = NULL, SDL_RendererFlip flipType=SDL_FLIP_NONE);
        void render_tile_set(vector<Tile> tile_set);
        void free();
};

#endif
