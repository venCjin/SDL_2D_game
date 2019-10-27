#include "Game.h"


Game::Game(std::ostream& log, int width, int height) : log(log), width(width), height(height)
{
	p1 = new SmoothPlayer(300, 300, 32, 32, 0.1f, 25);
	p2 = new Player(200, 200, 32, 32, 0.1f,25);
}

Game::~Game()
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

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	// if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER /*SDL_INIT_JOYSTICK*/) != 0)
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

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED /*| SDL_RENDERER_PRESENTVSYNC*/);
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
	map = new Map(lvl_w, lvl_h, renderer);
	if (!map->init())
	{
		GameLog::logSDLError(log, "Map");
		return;
	}

	cam = new SDL_Rect{ 0, 0, width, height };
	
	// Controllers init
	if (SDL_NumJoysticks() < 1)
	{
		SDL_SetError("No joysticks connected!"); 
		GameLog::logSDLError(log, "GameController");
	}
	else
	{
		if (SDL_IsGameController(0))
		{
			//Load Game Controller
			gameController = SDL_GameControllerOpen(0);
			if (gameController == nullptr)
				GameLog::logSDLError(log, "Unable to open game controller!");
			else
				log << "GameController connected!" << std::endl;
		}
		else
		{
			SDL_SetError("Your GameController is not supported by the game controller interface!");
			GameLog::logSDLError(log, "GameController");
		}
	}
	
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
			// p1.moveY(-32767);
			p1->moveY(-1.0f);
			break;
		case SDLK_DOWN:
			// p1.moveY(32768);
			p1->moveY(1.0f);
			break;
		case SDLK_LEFT:
			// p1.moveX(-32768);
			p1->moveX(-1.0f);
			break;
		case SDLK_RIGHT:
			// p1.moveX(32767);
			p1->moveX(1.0f);
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
			break;
		case SDLK_DOWN:
			p1->moveY(0.0f);
			break;
		case SDLK_LEFT:
			p1->moveX(0.0f);
			break;
		case SDLK_RIGHT:
			p1->moveX(0.0f);
			break;
		default:
			break;
		}
		break;

	case SDL_CONTROLLERAXISMOTION:
		/*if (e.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX)
		{
			if (abs(e.jaxis.value) > JOYSTICK_DEAD_ZONE)
				p2->moveX(e.jaxis.value);
			else
				p2->moveX(0);
		}
		if (e.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY)
		{
			if (abs(e.jaxis.value) > JOYSTICK_DEAD_ZONE)
				p2->moveY(e.jaxis.value);
			else
				p2->moveY(0);
		}
		break;*/
		if (e.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX)
		{
			if (e.jaxis.value > JOYSTICK_DEAD_ZONE)
				p2->moveX(1.0f);
			else if (e.jaxis.value < -JOYSTICK_DEAD_ZONE)
				p2->moveX(-1.0f);
			else
				p2->moveX(0.0f);
		}
		if (e.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY)
		{
			if (e.jaxis.value > JOYSTICK_DEAD_ZONE)
				p2->moveY(1.0f);
			else if (e.jaxis.value < -JOYSTICK_DEAD_ZONE)
				p2->moveY(-1.0f);
			else
				p2->moveY(0.0f);
		}
		break;

	default:
		break;
	}
}

void Game::update()
{
	// ograniczenie ruchu graczy (nie wyjda za kadr)
	if (p1->distanceX(*p2) < width-50
		&& p1->distanceY(*p2) < height-50
		&& p1->inLevelBounds(lvl_w, lvl_h))
		p1->update();
	if (p2->distanceX(*p1) < width-50
		&& p2->distanceY(*p1) < height-50
		&& p2->inLevelBounds(lvl_w, lvl_h))
		p2->update();

	//Center the camera over the dot
	cam->x = (p1->x + p2->x) / 2 - width / 2;
	cam->y = (p1->y + p2->y) / 2 - height / 2;
	//Keep the camera in lvl bounds
	if (cam->x < 0)
	{
		cam->x = 0;
	}
	if (cam->y < 0)
	{
		cam->y = 0;
	}
	if (cam->x > lvl_w - cam->w)
	{
		cam->x = lvl_w - cam->w;
	}
	if (cam->y > lvl_h - cam->h)
	{
		cam->y = lvl_h - cam->h;
	}
}

void Game::render()
{
	//Render the scene
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	map->render(cam);
	
	p1->render(cam, renderer);
	
	p2->render(cam, renderer);
	
	SDL_RenderPresent(renderer);
}

bool Game::running()
{
	return isRunning;
}

void Game::close()
{
	map->close();
	cleanup(window, renderer , gameController/*, background, playerother resources*/);

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}


