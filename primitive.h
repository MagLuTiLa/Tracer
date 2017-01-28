#pragma once
#include "ray.h"
#include "material.h"
#include "BVHNode.h"
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

	virtual glm::vec3 Color(const glm::vec3& position) { return material->texture[0]; };
	virtual AABB CalculateBounds() = 0;
	virtual glm::vec3 Centroid() = 0;
	virtual glm::vec3 RandomPointOn() = 0;
	glm::vec3 location;
	Material* material;
	int hax = 0;
	bool light = false;
};

