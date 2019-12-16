#include "Game.h"


Game::Game(std::ostream& log, int width, int height)
: log(log), width(width), height(height)
{
}

void Game::init(const char *title, bool fullscreen)
{
	isRunning = false;
	int flags = SDL_WINDOW_SHOWN;

	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		GameLog::logSDLError(log, "SDL_Init");
		return;
	}

	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
	if (window == nullptr)
	{
		GameLog::logSDLError(log, "CreateWindow");
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr)
	{
		GameLog::logSDLError(log, "CreateRenderer");
		return;
	}
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		GameLog::logSDLError(log, "IMG_Init");
		return;
	}

	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		// printf("Warning: Linear texture filtering not enabled!");
		SDL_SetError("Linear texture filtering not enabled!");
		GameLog::logSDLError(log, "Video");
	}

	// Load assets
	map = new Map();
	if (!map->init(renderer))
	{
		GameLog::logSDLError(log, "Map");
		return;
	}

	p1 = new Player(32, 256, /*32, 32,*/ 0.2f, 15);

	cam = new SDL_Rect{ 0, 0, width, height };
	
	// Load bg
	bg = new Layer(0.05f, "/images/bg.png", cam, renderer);
	// Load fg
	fg = new Layer(0.6f, "/images/fg3.png", cam, renderer);
	
	isRunning = true;
}

void Game::handleEvents()
{
	SDL_Event e;

	SDL_PollEvent(&e);

	switch (e.type)
	{
	case SDL_QUIT:
		isRunning = false;
		break;
		
	case SDL_KEYDOWN:
		switch (e.key.keysym.sym)
		{
		case SDLK_UP:
			p1->moveY(-1.0f);
			// p1->jump(-1.0f);
			break;
		case SDLK_DOWN:
			p1->moveY(1.0f);
			break;
		case SDLK_LEFT:
			bg->moveX(-1.0f);
			p1->moveX(-1.0f);
			fg->moveX(-1.0f);
			break;
		case SDLK_RIGHT:
			bg->moveX(1.0f);
			p1->moveX(1.0f);
			fg->moveX(1.0f);
			break;
		case SDLK_ESCAPE:
			isRunning = false;
			break;
		default:
			break;
		}
		break;

	case SDL_KEYUP:
		switch (e.key.keysym.sym)
		{
		case SDLK_UP:
			p1->moveY(0.0f);
			// p1->jump(0.0f);
			break;
		case SDLK_DOWN:
			p1->moveY(0.0f);
			break;
		case SDLK_LEFT:
			bg->moveX(0.0f);
			p1->moveX(0.0f);
			fg->moveX(0.0f);
			break;
		case SDLK_RIGHT:
			bg->moveX(0.0f);
			p1->moveX(0.0f);
			fg->moveX(0.0f);
			break;
		default:
			break;
		}
		break;
	}
}

void Game::update()
{
	bg->update();
	p1->update();
	fg->update();
		
	if (map->touchesGrass(*p1))
	{
		bg->revertUpdate();
		p1->revertUpdate();
		fg->revertUpdate();
	}
		

	// cam and player pos
	cam->x = p1->x - 32;
	if ( cam->x > 640 )
	{
		// printf("cam: %i %i\n", cam->x, cam->y);
		// printf("p1: %f %f\n", p1->x, p1->y);
		cam->x = 0;
		p1->x = 32;
	}
	else if ( cam->x < 0 )
	{
		// printf("cam: %i %i\n", cam->x, cam->y);
		// printf("p1: %f %f\n", p1->x, p1->y);
		cam->x = 640;
		p1->x = 640 + 32;
	}
}

void Game::render()
{
	//Render the scene
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	// Background
	bg->render(renderer);

	// Map
	map->render(cam, renderer);

	// Player
	p1->render(cam, renderer);

	// Foreground
	fg->render(renderer);
	
	SDL_RenderPresent(renderer);
}

bool Game::running()
{
	return isRunning;
}

void Game::close()
{
	map->close();
	bg->close();
	fg->close();
	cleanup(window, renderer);

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}
