#pragma once
#include "light.h"

class PointLight : public Light
{
public:
	PointLight(glm::vec3 loc, glm::vec3 c);
};

