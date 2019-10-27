#ifndef GAMELOG_H
#define GAMELOG_H
#include <ostream>
#include <SDL.h>

class GameLog
{
public:
	GameLog();
	~GameLog();

	static void logSDLError(std::ostream& os, const std::string& msg);
};

#endif
