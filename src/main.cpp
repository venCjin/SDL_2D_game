#include "Game.h"

int main(int argc, char **argv)
{
	Game* game = new Game(std::cout);

	game->init("Zad3", false);
	
	while (game->running())
	{
		game->handleEvents();
		game->update();
		game->render();
	}

	game->close();
	
	return 0;
}

/*
	To setup SDL2 in VS Community Project
	1.    Download SDL2-for-vc
	2.    Extract
	3.    Create Project (ConsoleApplication x86 / x64)
	4.    [Configuration == ALL] [Platform == ALL] Project Properties
	4.1   C/C++ > General > Additional Include Directories > (path\to\SDL2\include)
															 (path\to\SDL2_image\include)
	4.2   Linker > General > Additional Library Directories
	4.2.1 [Platform == Win32] (path\to\SDL2\lib\x86)
							  (path\to\SDL2_image\lib\x86)
	4.2.2 [Platform == x64] (path\to\SDL2\lib\x86)
							(path\to\SDL2_image\lib\x86)
	4.3   [Platform == ALL] Linker > Input > Additional Dependencies > (SDL2.lib SDL2main.lib SDL2_image.lib)
	4.4   Debug > Environment > (PATH=path\to\libs) where all DLLs required to run app are (ex. SDL2.dll)
*/
