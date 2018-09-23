#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstring>
#include <unistd.h>
#include <math.h>
#include <stdio.h>
using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );

		//Deallocates texture
		void free();

		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, SDL_RendererFlip flipType=SDL_FLIP_NONE );

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene sprites
SDL_Rect gSpriteClips[ 4 ];
SDL_Rect bullet[3];
LTexture gSpriteSheetTexture;

LTexture::LTexture(){
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture(){
	free();
}

/*bool LTexture::loadFromFile(string path){
	free();
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if(loadedSurface == NULL){
		cout<<"unable to load image";
	}
	else{
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		//newTexture = SDL_createTextureFromSurface(gRenderer, loadedSurface);
		if(newTexture == NULL){
			cout<<"unable to create texture";
		}
		else{
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}
		SDL_FreeSurface(loadedSurface);
	}
}*/

bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

void LTexture::render(int x, int y, SDL_Rect* clip, SDL_RendererFlip flipType){
	//SDL_Rect mySprite = {x, y, mWidth, mHeight};
	SDL_Rect mySprite = {x, y, 0, 0};
	if(clip != NULL){
		mySprite.w = clip->w;
		mySprite.h = clip->h;
	}

	SDL_RenderCopyEx(gRenderer, mTexture, clip, &mySprite, 0, NULL, flipType);
}

void LTexture::free(){
	if(mTexture != NULL){
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

int LTexture::getWidth(){
	return mWidth;
}

int LTexture::getHeight(){
	return mHeight;
}

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
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if(gRenderer == NULL){
				cout<<"renderer could not be created";
			}
			else{
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
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

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load sprite sheet texture
	if( !gSpriteSheetTexture.loadFromFile( "sprites.png" ) )
	{
		printf( "Failed to load sprite sheet texture!\n" );
		success = false;
	}
	else
	{
		gSpriteClips[0].x = 21;
		gSpriteClips[0].y = 308;
		gSpriteClips[0].w = 30;
		gSpriteClips[0].h = 42;

		gSpriteClips[1].x = 54;
		gSpriteClips[1].y = 308;
		gSpriteClips[1].w = 30;
		gSpriteClips[1].h = 42;
		
		gSpriteClips[2].x = 86;
		gSpriteClips[2].y = 308;
		gSpriteClips[2].w = 30;
		gSpriteClips[2].h = 42;

		gSpriteClips[3].x = 54;
		gSpriteClips[3].y = 308;
		gSpriteClips[3].w = 30;
		gSpriteClips[3].h = 42;

		bullet[0].x = 347;
		bullet[0].y = 652;
		bullet[0].w = 19;
		bullet[0].h = 22;


		bullet[1].x = 370;
		bullet[1].y = 651;
		bullet[1].w = 44;
		bullet[1].h = 37;


		bullet[2].x = 422;
		bullet[2].y = 658;
		bullet[2].w = 44;
		bullet[2].h = 23;
	}
	return success;
}

void close()
{
	//Free loaded images
	gSpriteSheetTexture.free();

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

bool check_collision(SDL_Rect Pbox, SDL_Rect wall){
	if(abs((Pbox.x+Pbox.w/2) - (wall.x+wall.w/2)) < abs(Pbox.w/2 + wall.w/2)){
		if(abs((Pbox.y+ Pbox.h/2) - (wall.y+wall.h/2)) < abs(Pbox.h/2 + wall.h/2)){
			return true;
		}
		else{
			return false;
		}
	}
	else{
		return false;
	}
}

int main(){
	if(!init()){
		cout<<"failed to initialize";
	}
	else{
		if(!loadMedia()){
			cout<<"failed to load image";
		}
		else{
			bool quit = false;
			SDL_Event e;
			
			int i=0, k=0, bvel=20, xvel=0, yvel=0 , Pvel=5, flag=0, flipFlag=1;
			int posX =250, posY = 250, tempx = 250, tempy=250;
			int bulletPosx = 400;
			int bulletPosy = 260;
			SDL_RendererFlip flipType = SDL_FLIP_NONE;
			SDL_RendererFlip bulletflipType = SDL_FLIP_HORIZONTAL;
			SDL_Rect wall = {350, 200, 60, 200};
			SDL_Rect Pbox = {250, 250, 29, 42};
			bool chk_col;	
			while(!quit){
				//vel=0;
				while(SDL_PollEvent(&e) != 0){
					if(e.type == SDL_QUIT){
						quit = true;
					}
					else{
						if(e.type == SDL_KEYDOWN && e.key.repeat==0){
							switch(e.key.keysym.sym){
								case SDLK_RIGHT: 
									xvel += Pvel;
									if(flipFlag==-1){
										flipFlag=1;
										flipType = SDL_FLIP_NONE;
									}
								break;
								case SDLK_LEFT: 
									xvel -= Pvel;
									if(flipFlag==1){
										flipFlag=-1;
										flipType = SDL_FLIP_HORIZONTAL;
									}
								break;
								case SDLK_UP: 
									yvel -= Pvel;
								break;
								case SDLK_DOWN: 
									yvel += Pvel;
								break;
							}
						}
						if(e.type == SDL_KEYUP){
							switch(e.key.keysym.sym){
								case SDLK_RIGHT: xvel -= Pvel;break;
								case SDLK_LEFT: xvel += Pvel;break;
								case SDLK_UP: yvel += Pvel;break;
								case SDLK_DOWN: yvel -= Pvel;break;
							}
						}
					}
				}
				bulletPosx -= bvel; 
				tempx = posX; tempy = posY;
				posX += xvel;
				posY += yvel;
				Pbox.x = posX;
				Pbox.y = posY;
				chk_col = check_collision(Pbox, wall);
				if(chk_col){
					//posX = wall.x - Pbox.w-8;
					//cout<<tempy+Pbox.h;
					if(tempy+Pbox.h <= wall.y){
						posY = wall.y-Pbox.h; 
					}
					else{
						//cout<<tempy+Pbox.h;
						if(tempy >= wall.y+wall.h){
							posY = wall.y+wall.h;
						}
						else{
							if(tempx < wall.x){
								posX = wall.x - Pbox.w;
							}
							else{
								posX = wall.x+wall.w;
							}
						}
					}
					Pbox.x = posX;
					Pbox.y = posY;
				}
				if(xvel==0){
					flag=0;
				}
				else{
					flag=1;
				}
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );
				
				SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
				SDL_RenderDrawRect( gRenderer, &wall );

				SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
				SDL_RenderDrawRect( gRenderer, &Pbox );
				gSpriteSheetTexture.render( posX, posY, &gSpriteClips[ i ], flipType );
				gSpriteSheetTexture.render( bulletPosx, bulletPosy, &bullet[ k ], bulletflipType );



				//Update screen
				SDL_RenderPresent( gRenderer );
				usleep(100000);
				i++;
				k++;
				if(flag==0){
					i=0;
				}
				if(i==4){
					//usleep(3000000);
					i=0;
				}
				if(k==3){
					k=2;
				}	
				if(bulletPosx<-50){
					bulletPosx = 640;
					k=0;
				}	
			}
		}
	}

	close();

	return 0;
}