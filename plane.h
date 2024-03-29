#pragma once
#include "primitive.h"

class Plane : public Primitive
{
public:
	Plane::Plane(glm::vec3 loc, glm::vec3 nor) :
		Primitive(loc),
		normal(nor)
	{}

	Plane::Plane(glm::vec3 loc, glm::vec3 nor, Material* m) :
		Primitive(loc, m),
		normal(nor)
	{}

	virtual void Intersect(Ray& ray);

	glm::vec3 normal;

	// Inherited via Primitive
	virtual glm::vec3 Sample(Ray & ray, Ray & lightRay) override;

	virtual glm::vec3 Normal(glm::vec3);

	virtual AABB CalculateBounds();

	virtual glm::vec3 Centroid();
};

