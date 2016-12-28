#pragma once
#define TRACECAP = 10;

#include "primitive.h"
class Primitive;
struct Ray
{
public:
	Ray():
		length(std::numeric_limits<float>::max()),
		color(glm::vec3(0, 0, 0)), 
		traceDepth(0)
	{};
	Ray(glm::vec3, glm::vec3 d);
	union
	{
		struct
		{
			glm::vec3 origin;
			int traceDepth;
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

	Primitive* hit = NULL;
	glm::vec3 color;
	bool inside = false;


	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

		void operator delete(void* p)
	{
		_mm_free(p);
	}
};

