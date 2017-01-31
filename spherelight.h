#pragma once
#include "light.h"
class SphereLight : public Light
{
public:
	SphereLight::SphereLight(Sphere * s, glm::vec3 c) :
		Light(s->Centroid(), c)
	{
		sphere = s;
	}

	// TODO might have to account for radius if using sphere
	inline virtual void getIllumination(const Ray& ray, Ray& outRay)
	{
		glm::vec3 point = ray.origin + ray.direction * (ray.length - EPSILON);
		float len = glm::length(location - point);
		outRay.direction = (location - point) / len;
		outRay.origin = point + outRay.direction * sphere->radius + EPSILON;
		outRay.color = color;
		outRay.length = len - sphere->radius - EPSILON;
	}

	Sphere* sphere;
};
