#pragma once
class Camera
{
public:
	Camera(glm::vec3, glm::vec3, glm::vec3, float);
	Ray ShootRay(float u, float v);
	void Update();
	void Translate(glm::vec3);

	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 up;

	float distance;
	glm::vec3 center;
	glm::vec3 right;
	glm::vec3 down;

	glm::vec3 topLeft;
	glm::vec3 topRight;
	glm::vec3 botLeft;

	glm::vec3 width;
	glm::vec3 height;
};

