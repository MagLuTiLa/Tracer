#pragma once
#include "light.h"

class PointLight : public Light
{
public:
	PointLight::PointLight(glm::vec3 loc, glm::vec3 c) :
		Light(loc, c)
	{}
	inline virtual Ray getIllumination(glm::vec3 point)
	{
		float len = glm::length(location - point);
		Ray shadowRay(point, (location - point) / len);
		shadowRay.color = color;
		shadowRay.length = len;
		return shadowRay;
	}
};

