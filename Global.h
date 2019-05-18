#ifndef GLOBAL_H
#define GLOBAL_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <cstring>
#include <vector>
#include "GameObj.h"
using namespace std;
class GameObj;

extern SDL_Renderer* gameRenderer;
extern SDL_Texture* TileSheetTexture;
extern SDL_Texture* EnemySheetTexture;
extern SDL_Texture* PlayerSheetTexture;
extern SDL_Texture* TileSheetTexture;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int LEVEL_WIDTH;
extern const int LEVEL_HEIGHT;
extern const double GRAVITY;
extern const double FREQUENCY;
extern const double TIME_STEP;
extern const double SECOND;
extern const double EPSILON;
extern const double SCALE;
extern bool BYPASS;
extern vector<GameObj*> object;
#endif