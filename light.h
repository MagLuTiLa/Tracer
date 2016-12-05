#pragma once
#include "ray.h"
class Light
{
public:
	Light::Light(glm::vec3 loc, glm::vec3 c) :
		location(loc),
		color(c)
	{}
	
	inline virtual Ray getIllumination(glm::vec3 point) = 0;

	glm::vec3 location;
	glm::vec3 color;
};