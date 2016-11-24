#pragma once
#include "ray.h"
#include "primitive.h"

class Sphere : public Primitive
{
public:
	Sphere(glm::vec3, float);
	virtual void Intersect(Ray& ray);
	float radius;
};