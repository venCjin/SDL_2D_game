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

	renderer = SDL_CreateRenderer(window, -1, 0 | SDL_RENDERER_ACCELERATED /*| SDL_RENDERER_PRESENTVSYNC*/);
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
	background = ResourceManager::loadTexture(resPath + "/images/stone_tile@128.png", renderer);
	player = ResourceManager::loadTexture(resPath + "/images/player@128.png", renderer);
	//Make sure they both loaded ok
	if (background == nullptr || player == nullptr)
	{
		SDL_SetError("Textures didn't load!");
		GameLog::logSDLError(log, "Video");
		return;
	}

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
			// std::cerr << "Warning: Joystick is not supported by the game controller interface!" << std::endl;
			SDL_SetError("Your GameController is not supported by the game controller interface!");
			GameLog::logSDLError(log, "GameController");

			joy = SDL_JoystickOpen(0);
			if (joy == nullptr)
				GameLog::logSDLError(log, "Unable to open joystick!");
			else
				log << "Joystick connected!" << std::endl;
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
			p1.moveY(-1.0f);
			break;
		case SDLK_DOWN:
			// p1.moveY(32768);
			p1.moveY(1.0f);
			break;
		case SDLK_LEFT:
			// p1.moveX(-32768);
			p1.moveX(-1.0f);
			break;
		case SDLK_RIGHT:
			// p1.moveX(32767);
			p1.moveX(1.0f);
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
			p1.moveY(0.0f);
			break;
		case SDLK_DOWN:
			p1.moveY(0.0f);
			break;
		case SDLK_LEFT:
			p1.moveX(0.0f);
			break;
		case SDLK_RIGHT:
			p1.moveX(0.0f);
			break;
		default:
			break;
		}
		break;

	case SDL_CONTROLLERAXISMOTION:
		/*if (e.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX)
		{
			if (abs(e.jaxis.value) > JOYSTICK_DEAD_ZONE)
				p2.moveX(e.jaxis.value);
			else
				p2.moveX(0);
		}
		if (e.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY)
		{
			if (abs(e.jaxis.value) > JOYSTICK_DEAD_ZONE)
				p2.moveY(e.jaxis.value);
			else
				p2.moveY(0);
		}
		break;*/
		if (e.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX)
		{
			if (e.jaxis.value > JOYSTICK_DEAD_ZONE)
				p2.moveX(1.0f);
			else if (e.jaxis.value < -JOYSTICK_DEAD_ZONE)
				p2.moveX(-1.0f);
			else
				p2.moveX(0.0f);
		}
		if (e.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY)
		{
			if (e.jaxis.value > JOYSTICK_DEAD_ZONE)
				p2.moveY(1.0f);
			else if (e.jaxis.value < -JOYSTICK_DEAD_ZONE)
				p2.moveY(-1.0f);
			else
				p2.moveY(0.0f);
		}
		break;

	default:
		break;
	}
}

void Game::update()
{
	if (p1.distance(p2) < playersMaxDistance)
		p1.update();
	
	if (p2.distance(p1) < playersMaxDistance)
		p2.update();
}

void Game::render()
{
	//Render the scene
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

/* BACKGROUND*/
	// renderTexture(background, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	for (int i = 0; i < width; i += TILE_SIZE)
	{
		for (int j = 0; j < height; j += TILE_SIZE)
		{
			ResourceManager::renderTexture(background, renderer, i, j, TILE_SIZE, TILE_SIZE);
		}
	}
/* BACKGROUND*/
	
/* PLAYER 1*/
	// renderTexture(player, renderer, p1.x, p1.y, p1.width, p1.height);
	//Render red filled quad
	SDL_Rect fillRect = { p1.x - 25, p1.y - 25, 50, 50 };
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_RenderFillRect(renderer, &fillRect);
/* PLAYER 1*/

/* PLAYER 2*/
	// renderTexture(player, renderer, p2.x, p2.y, p2.width, p2.height);
	ResourceManager::drawCircle(renderer, p2.x, p2.y, 20, 255, 0, 0, 127);
/* PLAYER 2*/
	
	SDL_RenderPresent(renderer);
}

bool Game::running()
{
	return isRunning;
}

void Game::close()
{
	cleanup(window, renderer , background, player/*other resources*/);

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}


