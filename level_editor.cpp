#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
using namespace std;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Current displayed texture
SDL_Texture* gTexture[3] = {NULL, NULL, NULL};
// for(int i=0; i<3; i++){
// 	gTexture[i] = NULL;
// }

bool init(){
	bool success = true;
	if(SDL_INIT_VIDEO < 0){
		cout<<"failed";
		success = false;
	}
	else{
		gWindow = SDL_CreateWindow("level editor", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if( gWindow == NULL ){
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if(gRenderer == NULL){
				cout<<"Error rendering";
				success = false;
			}
			else{
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				int imgFlags = IMG_INIT_PNG;
				if(!(IMG_Init(imgFlags)& imgFlags)){
					cout<<"Error loading png";
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

	//Load texture
	gTexture[0] = loadTexture( "freetileset/png/Tiles/1.png" );
	gTexture[1] = loadTexture( "freetileset/png/Tiles/2.png" );
	gTexture[2] = loadTexture( "freetileset/png/Tiles/3.png" );
	// if( gTexture == NULL )
	// {
	// 	printf( "Failed to load texture image!\n" );
	// 	success = false;
	// }

	//Nothing to load
	return success;
}

void close()
{
	//Free loaded image
	for(int i=0; i<3; i++){
		SDL_DestroyTexture( gTexture[i] );
		gTexture[i] = NULL;
	}
	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture( std::string path )
{
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
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}


void Rend(){
	//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				int x=0,y=0;

				//Top left corner viewport
				SDL_Rect topLeftViewport;
				topLeftViewport.x = 0;
				topLeftViewport.y = 400;
				topLeftViewport.w = SCREEN_WIDTH ;
				topLeftViewport.h = SCREEN_HEIGHT ;
				SDL_RenderSetViewport( gRenderer, &topLeftViewport );
				for (int j=0; j<3; j++){
					SDL_Rect clip = {0,0, 128, 128};
					SDL_Rect myClip = {x,y,64,64};
					//Render texture to screen
					SDL_RenderCopy( gRenderer, gTexture[0], &clip, &myClip );
					x = x+64;	
					//SDL_RenderCopy( gRenderer, gTexture[2], &clip, &myClip );
				}
				//gTexture.render(250, 250, &clip );


				//Top right viewport
				/*SDL_Rect topRightViewport;
				topRightViewport.x = SCREEN_WIDTH / 1.5;
				topRightViewport.y = 0;
				topRightViewport.w = SCREEN_WIDTH - SCREEN_WIDTH / 1.5;
				topRightViewport.h = SCREEN_HEIGHT / 1.5;
				SDL_RenderSetViewport( gRenderer, &topRightViewport );
				
				//Render texture to screen
				SDL_RenderCopy( gRenderer, gTexture, NULL, NULL );


				//Bottom viewport
				SDL_Rect bottomViewport;
				bottomViewport.x = 0;
				bottomViewport.y = SCREEN_HEIGHT / 1.5;
				bottomViewport.w = SCREEN_WIDTH;
				bottomViewport.h = SCREEN_HEIGHT - SCREEN_HEIGHT / 1.5;
				SDL_RenderSetViewport( gRenderer, &bottomViewport );

				
				//Render texture to screen
				SDL_RenderCopy( gRenderer, gTexture, NULL, NULL );*/


				//Update screen
				SDL_RenderPresent( gRenderer );
}

int main( int argc, char* args[] )
{
	int tile1=0;
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					else{
						if(e.type == SDL_MOUSEBUTTONDOWN){
							int x,y;
							SDL_GetMouseState(&x,&y);
							if(x<64 && (y>400 && y<464) ){
								tile1 = 1;
								cout<<"selected";
							}
							else{
								if(y<330){
									cout<<"youclicked";
								}
							}
						}
					}
				}
				Rend();
				
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}