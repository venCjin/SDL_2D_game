#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>

class Camera
{
	
public:
	Camera(float x, float y, int width, int height);
	~Camera() = default;

	float x, y;
	int width, height;

	void setCenter(int center_x, int center_y);
};

#endif

