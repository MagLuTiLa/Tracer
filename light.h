#pragma once
#include "template.h"
class Light
{
public:
	Light::Light(glm::vec3 loc, glm::vec3 c) :
		location(loc),
		color(c)
	{}
	
	inline virtual void getIllumination(const Ray& inRay, Ray& outRay) = 0;

	glm::vec3 location;
	glm::vec3 color;
};