#pragma once
class Ray
{
public:
	Ray(glm::vec3, glm::vec3 d);

	glm::vec3 origin;
	glm::vec3 direction;
	glm::vec3 colour;
	float length;
};

