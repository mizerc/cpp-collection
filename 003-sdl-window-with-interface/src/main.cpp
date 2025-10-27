#include "SDLWrapper.h"
#include "Game.h"

int
main()
{
	SDLWrapper sdl(800, 600);
	Game game(sdl);
	sdl.run(&game);
	return 0;
}
