#include "template.h"
#include "camera.h"


Camera::Camera(glm::vec3 p, glm::vec3 d, float dist)
{
	position = p;
	direction = d;
	distance = dist;

	center = position + direction * distance;

	topLeft = center + glm::vec3(-1, -1, 0);
	topRight = center + glm::vec3(1, -1, 0);
	botLeft = center + glm::vec3(-1, 1, 0);

	width = topRight - topLeft;
	height = botLeft - topLeft;

	/*
	up = vec3(0, 1, 0);
	right = glm::cross(direction, up);
	up = glm::cross(right, direction);
	
	topLeft = center + (0.5f * up) - (0.5f * right);
	topRight = topLeft + right;
	botLeft = center - up;

	width = glm::length(topRight - topLeft);
	height = glm::length(botLeft - topLeft);
	*/
	/*

	Ray ComputeCameraRay(int i, int j) {
	const float width = 512.0;  // pixels across
	const float height = 512.0;  // pixels high
	double normalized_i = (i / width) - 0.5;
	double normalized_j = (j / height) - 0.5;
	Vector3 image_point = normalized_i * camera_right +
	normalized_j * camera_up +
	camera_position + camera_direction;
	Vector3 ray_direction = image_point - camera_position;
	return Ray(camera_position, ray_direction);
	}
	*/

}

Ray Camera::ShootRay(float u, float v)
{
	glm::vec3 dir = topLeft + u*width + v*height - position;
	float length = glm::length(dir);
	dir = dir / length;
	return Ray(position, dir, std::numeric_limits<float>::max());
}
