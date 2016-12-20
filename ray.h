#pragma once
#define TRACECAP = 10;

#include "primitive.h"
class Primitive;
struct Ray
{
public:
	Ray(glm::vec3, glm::vec3 d);
	Primitive* hit = NULL;
	glm::vec3 color;
	bool inside = false;

	union
	{
		struct
		{
			glm::vec3 origin;
			int traceDepth = 0;
		};
		__m128 quadOrigin;
	};

	union
	{
		struct
		{
			glm::vec3 direction;
			float length;
		};
		__m128 quadDirection;
	};
};

