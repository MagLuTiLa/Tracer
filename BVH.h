#pragma once

#include <vector>
#include "primitive.h"
class BVH
{
public:
	BVH();

	void ConstructBVH(std::vector<Primitive*>*);
	void QuickSort(int l, int r, int axis = 0);
	void CalculateBounds(int);
	void Subdivide(int);
	void WriteToFile();
	
	void Traverse(Ray&, int, int* depth = NULL);
	void IntersectPrimitives(Ray&, int);

	std::vector<Primitive*>* primitives;
	int* indices;
	BVHNode* pool;
	AABB* bounds;
	glm::vec3* centroids;
	int poolPtr;
};

