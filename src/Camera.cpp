#include "Camera.h"

Camera::Camera(float x, float y, int width, int height)
	: x(x), y(y), width(width), height(height)
{
}
/*
@param center_x x coordinate of center of the object
@param center_y y coordinate of center of the object
 */
void Camera::setCenter(int center_x, int center_y)
{
	x = center_x - width / 2;
	y = center_y - height / 2;
}
