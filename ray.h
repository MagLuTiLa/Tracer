#pragma once
#define TRACECAP = 10;

#include "primitive.h"
class Primitive;
struct Ray
{
public:
	Ray(glm::vec3, glm::vec3 d);

	glm::vec3 origin;
	glm::vec3 direction;
	float length;
	Primitive* hit = NULL;
	glm::vec3 color;
	int traceDepth = 0;
};

