#pragma once
#include "ray.h"

class Primitive
{
public:
	Primitive::Primitive(glm::vec3 loc) :
		location(loc)
	{}

	virtual void Intersect(Ray& ray) = 0;
	glm::vec3 location;
};

