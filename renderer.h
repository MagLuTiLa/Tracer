#pragma once

#include <vector>
#include "primitive.h"

class Renderer
{
public:
	Renderer();

	void Init();

	glm::vec3 TraceRay(Ray&);
	glm::vec3 DirectIllumination(Ray);
	glm::vec3 Reflect(Ray);
	glm::vec3 Refract(Ray, float, float);

	void AddPrimitive(Primitive*);
	void AddLight(Light*);

private:
	std::vector<Primitive*> primitives;
	std::vector<Light*> lights;
};

