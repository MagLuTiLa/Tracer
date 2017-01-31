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
	glm::vec3 IndirectIllumination(Ray&);
	glm::vec3 Reflect(Ray&);
	glm::vec3 Refract(Ray&, float, float);

	BVH bvh;
	bool depthMode = false;
	bool bvhMode = true;

	void AddPrimitive(Primitive*);
	void AddLight(Light*);
	glm::vec3 WorldToLocal(glm::vec3 world, glm::vec3 normal);
	float Randamonium();

	glm::vec3 PhongBRDF();

	glm::vec3 PhongBRDF(float alpha);

private:
	std::vector<Primitive*> primitives;
	std::vector<Light*> lights;
	int prime1 = 114818411;
	int prime2 = 782595287;
	unsigned int seed = rand() * prime1 * prime2;
};

