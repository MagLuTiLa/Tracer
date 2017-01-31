#pragma once
#include "light.h"
#include "triangle.h"
class TriangleLight : public Light
{
public:
	TriangleLight::TriangleLight(Triangle * t, glm::vec3 c) :
		Light(t->Centroid(), c)
	{
		triangle = t;
	}

	// TODO might have to account for radius if using sphere
	inline virtual void getIllumination(const Ray& ray, Ray& outRay)
	{
		glm::vec3 point = ray.origin + ray.direction * (ray.length - EPSILON);
		float len = glm::length(location - point);
		outRay.origin = point;
		outRay.direction = (location - point) / len;
		outRay.color = color;
		outRay.length = len - EPSILON;
	}

	Triangle* triangle;
};