#pragma once
#include "light.h"

class PointLight : public Light
{
public:
	PointLight::PointLight(glm::vec3 loc, glm::vec3 c) :
		Light(loc, c)
	{}

	inline virtual void getIllumination(const Ray& ray, Ray& outRay)
	{
		glm::vec3 point = ray.origin + ray.direction * ray.length;
		point += 0.0001f * ray.hit->Normal(point);
		float len = glm::length(location - point);
		outRay.origin = point;
		outRay.direction = (location - point) / len;
		outRay.color = color;
		outRay.length = len;
	}
};

