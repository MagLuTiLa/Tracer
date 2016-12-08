#pragma once
#include "primitive.h"
__declspec(align(64)) class Triangle :
	public Primitive
{
public:
	Triangle() : Primitive(glm::vec3())
	{
	}

	Triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) : Primitive(v1), location2(v2), location3(v3)
	{
		CalculateNormal();
	}

	inline void CalculateNormal()
	{
		normal = glm::normalize(glm::cross(location2 - location, location3 - location));
	}

	void* operator new[](size_t i)
	{
		return _mm_malloc(i, 64);
	}

	void operator delete[](void* p)
	{
		_mm_free(p);
	}

	glm::vec3 normal;
	glm::vec3 location2;
	glm::vec3 location3;

	glm::vec2* uv = NULL;

	~Triangle();

	// Inherited via Primitive
	virtual void Intersect(Ray & ray) override;
	virtual glm::vec3 Sample(Ray & ray, Ray & lightRay) override;
	virtual glm::vec3 Normal(glm::vec3 loc) override;
	virtual glm::vec3 Color(const glm::vec3& position) override;
	virtual AABB CalculateBounds();
};

