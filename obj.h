#pragma once
#include "triangle.h"
#include "primitive.h"

void LoadObj(const char* fileName, std::vector<Primitive*> &primitives, const glm::mat4 &trans = mat4());
