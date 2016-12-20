#pragma once

#include <vector>
#include "primitive.h"
#include "BVHNode.h"
#include "BVH.h"


class Renderer
{
public:
	Renderer();

	void Init();

	glm::vec3 TraceRay(Ray&);
	glm::vec3 DirectIllumination(Ray&);
	glm::vec3 Reflect(Ray&);
	glm::vec3 Refract(Ray&, float, float);

	BVH bvh;

	void AddPrimitive(Primitive*);
	void AddLight(Light*);

private:
	std::vector<Primitive*> primitives;
	std::vector<Light*> lights;
};

