#pragma once
#include "ray.h"
#include "material.h"

class Primitive
{
public:
	Primitive::Primitive(glm::vec3 loc) :
		location(loc),
		material(Material(glm::vec3(1.0f,1.0f,1.0f)))
	{}

	Primitive::Primitive(glm::vec3 loc, Material m) :
		location(loc),
		material(m)
	{}

	virtual void Intersect(Ray& ray) = 0;
	virtual glm::vec3 Sample(Ray& ray, Ray& lightRay) = 0;
	virtual glm::vec3 Normal(glm::vec3 loc) = 0;

	glm::vec3 Color() { return material.color; };

	glm::vec3 location;
	Material material;
};

