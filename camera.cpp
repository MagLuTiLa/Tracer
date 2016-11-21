#include "template.h"
#include "camera.h"


Camera::Camera(glm::vec3 p, glm::vec3 d, float dist)
{
	position = p;
	direction = d;
	distance = dist;

	center = position + direction * distance;

	// Used for the "screen" through where rays are shot
	topLeft = center + glm::vec3(-1, -1, 0);
	topRight = center + glm::vec3(1, -1, 0);
	botLeft = center + glm::vec3(-1, 1, 0);

	width = topRight - topLeft;
	height = botLeft - topLeft;
}

Ray Camera::ShootRay(float u, float v)
{
	glm::vec3 dir = topLeft + u*width + v*height - position;
	float length = glm::length(dir);
	dir = dir / length;
	return Ray(position, dir, std::numeric_limits<float>::max());
}
