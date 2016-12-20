#pragma once

#include <vector>
#include "primitive.h"
class BVH
{
public:
	BVH();

	void ConstructBVH(std::vector<Primitive*>*, int);
	void QuickSort(int, int);
	void CalculateBounds(int);
	void Subdivide(int);
	void Partition(int);
		void Traverse(Ray&, int);	void IntersectPrimitives(Ray&, int);
	std::vector<Primitive*>* primitives;
	int* indices;
	BVHNode* pool;
	AABB* bounds;
	glm::vec3* centroids;
	int poolPtr;
};

