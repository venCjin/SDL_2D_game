#include "GameLog.h"


GameLog::GameLog()
{
}

GameLog::~GameLog()
{
}

/**
* Log an SDL error with some error message to the output stream of our choice
* @param os The output stream to write the message to
* @param msg The error message to write, format will be msg Error: SDL_GetError()
*/
void GameLog::logSDLError(std::ostream& os, const std::string& msg)
{
	os << msg << "Error: " << SDL_GetError() << std::endl;
}