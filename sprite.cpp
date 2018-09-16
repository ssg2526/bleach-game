#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstring>
#include <unistd.h>
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
		void render( int x, int y, SDL_Rect* clip = NULL );

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

void LTexture::render(int x, int y, SDL_Rect* clip){
	SDL_Rect mySprite = {x, y, mWidth, mHeight};
	if(clip != NULL){
		mySprite.w = clip->w*2;
		mySprite.h = clip->h*2;
	}

	SDL_RenderCopy(gRenderer, mTexture, clip, &mySprite);
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
		gSpriteClips[0].x = 18;
		gSpriteClips[0].y = 484;
		gSpriteClips[0].w = 40;
		gSpriteClips[0].h = 45;

		gSpriteClips[1].x = 59;
		gSpriteClips[1].y = 485;
		gSpriteClips[1].w = 40;
		gSpriteClips[1].h = 45;
		
		gSpriteClips[2].x = 103;
		gSpriteClips[2].y = 482;
		gSpriteClips[2].w = 40;
		gSpriteClips[2].h = 45;

		gSpriteClips[3].x = 146;
		gSpriteClips[3].y = 490;
		gSpriteClips[3].w = 40;
		gSpriteClips[3].h = 45;
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
			
			int i=0;
			while(!quit){
				while(SDL_PollEvent(&e) != 0){
					if(e.type == SDL_QUIT){
						quit = true;
					}
				}
				
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );
				

				gSpriteSheetTexture.render( 250, 250, &gSpriteClips[ i ] );

				//Render top right sprite
				//gSpriteSheetTexture.render( 250, 250, &gSpriteClips[ 1 ] );

				//Render bottom left sprite
				//gSpriteSheetTexture.render( 250, 250, &gSpriteClips[ 2 ] );

				//Render bottom right sprite
				//gSpriteSheetTexture.render( 250, 250, &gSpriteClips[ 3 ] );

				//Update screen
				SDL_RenderPresent( gRenderer );
				usleep(100000);
				i++;
				if(i==1){
					//usleep(3000000);
					i=0;
				}		
			}
		}
	}

	close();

	return 0;
}