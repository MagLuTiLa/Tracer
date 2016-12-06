#pragma once
#include "ray.h"
class Camera
{
public:
	Camera();
	Ray ShootRay(float u, float v);

	void Reset();
	void Update();

	void Translate(glm::vec3);

	void Horizontal(float);
	void Vertical(float);
	void Axial(float);

	void Jaw(float);
	void Pitch(float);

	void HorStretch(float);
	void VerStretch(float);

	void Zoom(float);

	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 up;

	float distance;
	float horStretch = 1.f;
	float verStretch = 1.f;
	glm::vec3 center;
	glm::vec3 right;
	glm::vec3 down;

	glm::vec3 topLeft;
	glm::vec3 topRight;
	glm::vec3 botLeft;

	glm::vec3 width;
	glm::vec3 height;
};

