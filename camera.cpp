#include "template.h"
#include "camera.h"


Camera::Camera(glm::vec3 p, glm::vec3 d, float dist):
	position(p),
	direction(d),
	distance(dist)
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
	glm::vec3 dir = topLeft + u*width + v*height - position;
	float length = glm::length(dir);
	dir = dir / length;
	return Ray(position, dir);
}
