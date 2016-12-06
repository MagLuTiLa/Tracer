#include "template.h"
#include "camera.h"


Camera::Camera(glm::vec3 p, glm::vec3 lookAt, float dist):
	position(p),
	distance(dist)
{
	direction = glm::normalize(lookAt - position);
	Update();
}

void Camera::Update()
{
	center = position + direction * distance;

	float stdValue = 1.0f;
	float ratio;
	if (SCRWIDTH > SCRHEIGHT)
	{
		ratio = (float)SCRWIDTH / (float)SCRHEIGHT;
		topLeft = center + glm::vec3(-ratio, -1, 0);
		topRight = center + glm::vec3(ratio, -1, 0);
		botLeft = center + glm::vec3(-ratio, 1, 0);
	}
	else
	{
		ratio = (float)SCRHEIGHT / (float)SCRWIDTH;
		topLeft = center + glm::vec3(-1, -ratio, 0);
		topRight = center + glm::vec3(1, -ratio, 0);
		botLeft = center + glm::vec3(-1, ratio, 0);
	}

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
