#pragma once
#include "triangle.h"
#include "primitive.h"

int LoadObj(const char* fileName, std::vector<Primitive*> &primitives, const glm::mat4 &trans = mat4(), Triangle** triangleOut = NULL);
