#pragma once

#include <vector>
#include "primitive.h"
#include "BVHNode.h"
#include "BVH.h"


class Renderer
{
public:
	Renderer();

	int Init();

	glm::vec3 TraceRay(Ray&);
	glm::vec3 DirectIllumination(Ray&);
	glm::vec3 Reflect(Ray&);
	glm::vec3 Refract(Ray&, float, float);

	BVH bvh;
	bool depthMode = false;
	bool bvhMode = true;

	void AddPrimitive(Primitive*);
	void AddLight(Light*);
	float Randamonium();

	glm::vec3 PhongBRDF();

private:
	std::vector<Primitive*> primitives;
	std::vector<Light*> lights;
	int seed = rand();
};

