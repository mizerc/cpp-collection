#include <iostream>
#include "constants.h"
#include "SDLWrapper.h"
#include "App.h"

int
main()
{
	std::cout << "hi\n";
	SDLWrap sdl;
	if(!sdl.initialize_window()) {
		std::cerr << "Failed to initialize SDL window\n";
		return -1;
	}

	App myapp(sdl);
	myapp.setup();
	while(myapp.is_running()) {
		sdl.process_input();
		myapp.handle_input();
		sdl.update_deltatime();
		float dt = sdl.get_deltatime();
		myapp.update(dt);
		sdl.clear();
		myapp.render();
		sdl.present();
		SDL_Delay(FRAME_TARGET_TIME);
	}

	sdl.destroy_window();
	std::cout << "bye\n";
	return 0;
}
