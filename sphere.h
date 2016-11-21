#pragma once
#include "ray.h"

class Sphere
{
public:
	Sphere(glm::vec3, float);
	void Intersect(Ray& ray);
	glm::vec3 location;
	float radius;
};