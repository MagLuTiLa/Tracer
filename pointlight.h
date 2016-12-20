#pragma once
#include "light.h"

class PointLight : public Light
{
public:
	PointLight::PointLight(glm::vec3 loc, glm::vec3 c) :
		Light(loc, c)
	{}

	inline virtual void getIllumination(glm::vec3 point, Ray& outRay)
	{
		float len = glm::length(location - point);
		outRay.origin = point;
		outRay.direction = (location - point) / len;
		outRay.color = color;
		outRay.length = len;

	}
};

