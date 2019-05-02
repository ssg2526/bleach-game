#ifndef GLOBAL_H
#define GLOBAL_H
//#include <iostram>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstring>
#include <vector>

extern SDL_Renderer* gameRenderer;
extern SDL_Texture* TileSheetTexture;
extern SDL_Texture* EnemySheetTexture;
extern SDL_Texture* PlayerSheetTexture;
extern SDL_Texture* TileSheetTexture;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int LEVEL_WIDTH;
extern const int LEVEL_HEIGHT;
extern const float GRAVITY;
extern const float FREQUENCY;
extern const float TIME_STEP;
extern const float SECOND;
extern const float EPSILON;
extern const float SCALE;
extern bool BYPASS;
#endif