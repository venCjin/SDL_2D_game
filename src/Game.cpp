#include "Game.h"


Game::Game(std::ostream& log, int width, int height) : log(log), width(width), height(height)
{
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

	// if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER /*SDL_INIT_JOYSTICK*/) != 0)
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
	map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, renderer);
	if (!map->init())
	{
		GameLog::logSDLError(log, "Map");
		return;
	}

	// players after map
	int x, y;
	map->genStartPoint(x, y);
	p1 = new SmoothPlayer(x-5, y-5, /*32, 32,*/ 0.2f, 15);
	p2 = new Player(x+5, y+5, /*32, 32,*/ 0.2f, 15);
	
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
		/*if (e.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX)
		{
			if (abs(e.jaxis.value) > JOYSTICK_DEAD_ZONE)
				p2->moveX(e.jaxis.value);
			else
				p2->moveX(0);
		}
		if (e.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
		{
			if (abs(e.jaxis.value) > JOYSTICK_DEAD_ZONE)
				p2->moveY(e.jaxis.value);
			else
				p2->moveY(0);
		}
		break;*/
		if (e.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX)
		{
			if (e.jaxis.value > JOYSTICK_DEAD_ZONE)
				p2->moveX(1.0f);
			else if (e.jaxis.value < -JOYSTICK_DEAD_ZONE)
				p2->moveX(-1.0f);
			else
				p2->moveX(0.0f);
		}
		if (e.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
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
	if(map->isWin(*p1))
		isRunning = false;
	
	if (map->isWin(*p2))
		isRunning = false;
	
	// ograniczenie ruchu graczy (nie wyjda za kadr)
	p1->update();
	if (p1->distanceX(*p2) > width - (p1->radius + p2->radius)
		|| p1->distanceY(*p2) > height - (p1->radius + p2->radius)
		|| p1->outOfLevelBounds(LEVEL_WIDTH, LEVEL_HEIGHT)
		|| map->touchesWall(*p1)
		)
		p1->revertUpdate();

	p2->update();
	if (p2->distanceX(*p1) > width - (p1->radius + p2->radius)
		|| p2->distanceY(*p1) > height - (p1->radius + p2->radius)
		|| p2->outOfLevelBounds(LEVEL_WIDTH, LEVEL_HEIGHT)
		|| map->touchesWall(*p2)
		)
		p2->revertUpdate();

	//Center the camera between players
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
	if (cam->x > LEVEL_WIDTH - cam->w)
	{
		cam->x = LEVEL_WIDTH - cam->w;
	}
	if (cam->y > LEVEL_HEIGHT - cam->h)
	{
		cam->y = LEVEL_HEIGHT - cam->h;
	}
}

void Game::render()
{
	//Render the scene
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	map->render(cam);
	
	p1->render(cam, renderer);
	
	p2->render(cam, renderer);

	// hint to win
	map->showDirection(cam);
	
	SDL_RenderPresent(renderer);
}

bool Game::running()
{
	return isRunning;
}

void Game::close()
{
	map->close();
	cleanup(window, renderer , gameController/*, background, player, other resources*/);

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}


