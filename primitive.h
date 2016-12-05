#pragma once
#include "ray.h"
#include "material.h"
class Primitive
{
public:
	Primitive::Primitive(glm::vec3 loc) : Primitive(loc, BASE_MATERIAL)
	{}

	Primitive::Primitive(glm::vec3 loc, Material* m) :
		location(loc),
		material(m)
	{}

	virtual void Intersect(Ray& ray) = 0;
	virtual glm::vec3 Sample(Ray& ray, Ray& lightRay) = 0;
	virtual glm::vec3 Normal(glm::vec3 loc) = 0;

	glm::vec3 Color() { return material->Color(glm::vec2(0,0)); };

	glm::vec3 location;
	Material* material;
};

