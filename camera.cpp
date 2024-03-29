#include "template.h"
#include "camera.h"


Camera::Camera()
{
	Reset();
	/*direction = glm::normalize(lookAt - position);
	Update();*/
}

void Camera::Reset()
{
	position = glm::vec3(0, 0, 0);
	vec3 lookAt(0, 0, 1);
	//vec3 lookAt(1, 2, 3);
	direction = glm::normalize(lookAt - position);
	up = glm::vec3(0, -1, 0);

	distance = 1;
	horStretch = 1;
	verStretch = 1;
	
	Update();
}

void Camera::Update()
{
	center = position + direction * distance;
	right = glm::cross(up, direction);
	down = -glm::cross(direction, right);

	glm::vec3 hor;
	glm::vec3 ver;
	if (SCRWIDTH > SCRHEIGHT)
	{
		hor = -((float)SCRWIDTH / (float)SCRHEIGHT) * right * horStretch;
		ver = down * verStretch;
	}
	else
	{
		hor = -right * verStretch;
		ver = ((float)SCRHEIGHT / (float)SCRWIDTH) * down * verStretch;
	}

	topLeft = center - hor - ver;
	glm::vec3 topRight = center + hor - ver;
	glm::vec3 botLeft = center - hor + ver;

	// Used for the "screen" through where rays are shot
	width = topRight - topLeft;
	height = botLeft - topLeft;
}

void Camera::Translate(glm::vec3 t)
{
	position += t;
	Update();
}

void Camera::Horizontal(float inc)
{
	position += -inc * right;
	Update();
}

void Camera::Vertical(float inc)
{
	position += -inc * down;
	Update();
}

void Camera::Axial(float inc)
{
	position += inc * direction;
	Update();
}

void Camera::Jaw(float angle)
{
	float X = direction.x;
	float Z = direction.z;

	float rad = angle * PI / 180;
	float c = glm::cos(rad);
	float s = glm::sin(rad);

	float nX = X*c - Z*s;
	float nZ = Z*c + X*s;

	direction.x = nX;
	direction.z = nZ;

	Update();
}

void Camera::Pitch(float angle)
{
	float Y = direction.y;
	float Z = direction.z;

	float rad = angle * PI / 180;
	float c = glm::cos(rad);
	float s = glm::sin(rad);

	float nY = Y*c - Z*s;
	float nZ = Z*c + Y*s;

	direction.y = nY;
	direction.z = nZ;

	Update();
}

void Camera::HorStretch(float inc)
{
	if (horStretch + inc > 0)
	{
		horStretch += inc;
		Update();
	}
}

void Camera::VerStretch(float inc)
{
	if (verStretch + inc > 0)
	{
		verStretch += inc;
		Update();
	}
}

void Camera::Zoom(float inc)
{
	if (distance + inc > 0)
	{
		distance += inc;
		Update();
	}
}

void Camera::ShootRay(float u, float v, Ray& outRay)
{
	glm::vec3 dir = glm::normalize(topLeft + u*width + v*height - position);
	outRay.origin = position;
	outRay.direction = dir;
}