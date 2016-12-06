#include "template.h"
#include "camera.h"


Camera::Camera(glm::vec3 p, glm::vec3 lookAt, glm::vec3 u, float dist):
	position(p),
	distance(dist),
	up(u)
{
	direction = glm::normalize(lookAt - position);
	Update();
}

void Camera::Update()
{
	center = position + direction * distance;
	right = glm::cross(up, direction);
	down = -glm::cross(direction, right);

	float stdValue = 1.0f;
	glm::vec3 ratio;
	if (SCRWIDTH > SCRHEIGHT)
	{
		float r = ((float)SCRWIDTH / (float)SCRHEIGHT);
		ratio = r * right;

		topLeft = center - ratio - down;
		topRight = center + ratio - down;
		botLeft = center - ratio + down;
		/*topLeft = center + glm::vec3(-ratio, -1, 0);
		topRight = center + glm::vec3(ratio, -1, 0);
		botLeft = center + glm::vec3(-ratio, 1, 0);*/
	}
	else
	{
		ratio = ((float)SCRHEIGHT / (float)SCRWIDTH) * down;

		topLeft = center - right - ratio;
		topRight = center + right - ratio;
		botLeft = center - right + ratio;
		/*topLeft = center + glm::vec3(-1, -ratio, 0);
		topRight = center + glm::vec3(1, -ratio, 0);
		botLeft = center + glm::vec3(-1, ratio, 0);*/
	}
	/*
	topLeft = center - right - down;
	topRight = center + right - down;
	botLeft = center - right + down;*/

	// Used for the "screen" through where rays are shot
	width = topRight - topLeft;
	height = botLeft - topLeft;
}

Ray Camera::ShootRay(float u, float v)
{
	glm::vec3 dir = glm::normalize(topLeft + u*width + v*height - position);
	return Ray(position, dir);
}

void Camera::MoveForward()
{
	//distance += 0.1f;
	position += glm::vec3(0, 0, 0.1f);
	Update();
}

void Camera::MoveBack()
{
	//distance -= 0.1f;
	position -= glm::vec3(0, 0, 0.1f);
	Update();
}
