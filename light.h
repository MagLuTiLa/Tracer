#pragma once
class Light
{
public:
	Light::Light(glm::vec3 loc, glm::vec3 c) :
		location(loc),
		color(c)
	{}

	glm::vec3 location;
	glm::vec3 color;
};