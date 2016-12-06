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

	if (SCRWIDTH > SCRHEIGHT)
	{
		glm::vec3 ratio = -((float)SCRWIDTH / (float)SCRHEIGHT) * right;

		topLeft = center - ratio - down;
		topRight = center + ratio - down;
		botLeft = center - ratio + down;
	}
	else
	{
		glm::vec3 ratio = ((float)SCRHEIGHT / (float)SCRWIDTH) * down;

		topLeft = center + right - ratio;
		topRight = center - right - ratio;
		botLeft = center + right + ratio;
	}

	// Used for the "screen" through where rays are shot
	width = topRight - topLeft;
	height = botLeft - topLeft;
}

void Camera::Translate(glm::vec3 t)
{
	position += t;
	Update();
}

Ray Camera::ShootRay(float u, float v)
{
	glm::vec3 dir = glm::normalize(topLeft + u*width + v*height - position);
	return Ray(position, dir);
}