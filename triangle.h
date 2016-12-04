#pragma once
#include "primitive.h"
__declspec(align(64)) class Triangle :
	public Primitive
{
public:
	int dummy;
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

	glm::vec3 normal;
	glm::vec3 location2;
	glm::vec3 location3;

	glm::vec2* uv = NULL;

	~Triangle();

	// Inherited via Primitive
	virtual void Intersect(Ray & ray) override;
	virtual glm::vec3 Sample(Ray & ray, Ray & lightRay) override;
	virtual glm::vec3 Normal(glm::vec3 loc) override;
};

