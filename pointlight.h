#pragma once
class PointLight
{
public:
	PointLight(glm::vec3 loc, int c);

	glm::vec3 location;
	int color;
};

