#pragma once
#include "primitive.h"
class Triangle :
	public Primitive
{
public:
	Triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) : Primitive(v1), location2(v2), location3(v3)
	{
		normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));
	}
	glm::vec3 normal;
	glm::vec3 location2;
	glm::vec3 location3;

	~Triangle();

	// Inherited via Primitive
	virtual void Intersect(Ray & ray) override;
	virtual glm::vec3 Sample(Ray & ray, Ray & lightRay) override;
};

