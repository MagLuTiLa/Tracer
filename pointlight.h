#pragma once
class PointLight
{
public:
	PointLight(glm::vec3 loc, glm::vec3 c);

	glm::vec3 location;
	glm::vec3 color;
};

