#pragma once
class Ray
{
public:
	Ray(glm::vec3, glm::vec3 d, float l);

	glm::vec3 origin;
	glm::vec3 direction;
	float length;
};

