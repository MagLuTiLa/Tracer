#pragma once
#include "ray.h"

class Primitive
{
public:
	Primitive::Primitive(glm::vec3 loc) :
		location(loc),
		color(glm::vec3(1.0f,1.0f,1.0f))
	{}

	virtual void Intersect(Ray& ray) = 0;
	virtual glm::vec3 Sample(Ray& ray, Ray& lightRay) = 0;

	glm::vec3 color;
	glm::vec3 location;
};

