#pragma once
#include "primitive.h"

class Plane : public Primitive
{
public:
	Plane(glm::vec3 loc, glm::vec3 nor);
	virtual void Intersect(Ray& ray);

	glm::vec3 normal;

	// Inherited via Primitive
	virtual glm::vec3 Sample(Ray & ray, Ray & lightRay) override;
};

