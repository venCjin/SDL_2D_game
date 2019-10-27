#ifndef PLAYER_H
#define PLAYER_H

#include <cmath>

class Player
{
private:
	// float directionX = 0.0f; // zakres od -1 do 1
	// float directionY = 0.0f; // zakres od -1 do 1

public:
	float speed;
	float directionX = 0;
	float directionY = 0;
	float x, y;
	unsigned width, height;

	Player(float x, float y, unsigned width, unsigned height, float speed);
	~Player();

	void start();
	virtual void update();
	// void moveX(float direction_x);
	// void moveY(float direction_y);
	void moveX(int direction_x);
	void moveY(int direction_y);
	int distance(Player &p2);
};

#endif

#ifndef SMOOTH_PLAYER_H
#define SMOOTH_PLAYER_H
class SmoothPlayer : public Player
{
public:
	SmoothPlayer(float x, float y, unsigned width, unsigned height, float speed);

	float lastDirectionX = 0.0f;
	float lastDirectionY = 0.0f;
	const float epsilon = 0.1f;
	void update() override;
};

#endif

