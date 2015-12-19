
#include <time.h>
#include <stdlib.h>


#include "2048.h"

#include "sdl_util.h"


#include "SDL.h"

void gameover(int nr,int points)
{
	if(nr)
		return;

	char filename[256];
	int highscore=0;

	snprintf(filename, sizeof filename, "%s%s", SDL_GetPrefPath("net.exse.seb","2048"), "highscore");
	SDL_RWops *file = SDL_RWFromFile(filename, "rb");
	if(file)
	{
		highscore=SDL_ReadBE32(file);
		SDL_RWclose(file);
	}

	if(points > highscore)
	{
		file = SDL_RWFromFile(filename, "wb");
		if(file)
		{
			SDL_WriteBE32(file,points);
			SDL_RWclose(file);
		}
	}
}


int main(int argc __attribute__((__unused__)), char *argv[] __attribute__((__unused__))) 
{
//	srand(time(NULL));
	
	int zoom = 90;

	unsigned int* pixelbuffer = sdl_init(GRIDSIZE*zoom, GRIDSIZE*zoom,"2048",20);

	//set_gameover_callback(gameover);

	int limiter=0;


	while(sdl_handle_events(pixelbuffer)) //limits loop to 60fps
	{

		while(sdl_limit_fps(&limiter,10))
		{
			update_2048(getkey_single);
			release_upped_keys();
		}

		render_2048(pixelbuffer,zoom);

	}
	sdl_deinit();
	return 0;
}

