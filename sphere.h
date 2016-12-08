#pragma once
#include "ray.h"
#include "primitive.h"

class Sphere : public Primitive
{
public:
	Sphere::Sphere(glm::vec3 loc, float r) :
		Primitive(loc),
		radius(r)
	{
		hax = 1;
	};

	Sphere::Sphere(glm::vec3 loc, float r, Material* m) :
		Primitive(loc, m),
		radius(r)
	{
		hax = 1;
	};

	virtual void Intersect(Ray& ray);
	virtual glm::vec3 Sample(Ray & ray, Ray & lightRay);
	virtual glm::vec3 Normal(glm::vec3);
	virtual AABB CalculateBounds();
	float radius;
};