#pragma once
#include "primitive.h"
class Primitive;
class Ray
{
public:
	Ray(glm::vec3, glm::vec3 d);

	glm::vec3 origin;
	glm::vec3 direction;
	float length;
	Primitive* hit;
};

