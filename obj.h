#pragma once
#include "triangle.h"
#include "material.h"
#include "primitive.h"

int LoadObj(const char* fileName, std::vector<Primitive*> &primitives, Material* m, const glm::mat4 &trans = mat4(), Triangle** triangleOut = NULL);

inline int LoadObj(const char* fileName, std::vector<Primitive*> &primitives, const glm::mat4 &trans = mat4(), Triangle** triangleOut = NULL)
{
	return LoadObj(fileName, primitives, BASE_MATERIAL, trans, triangleOut);
}
