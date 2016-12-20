#include "template.h"
#include "BVH.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>


BVH::BVH()
{
}

void BVH::ConstructBVH(std::vector<Primitive*>* p, int count)
{
	primitives = p;
	// create index array
	indices = new int[count];
	for (int i = 0; i < count; i++) 
		indices[i] = i;
	// allocate BVH root node
	pool = reinterpret_cast<BVHNode*>(_mm_malloc(count*2*sizeof(BVHNode), 64));// [count * 2];
	BVHNode* root = &pool[0];
	poolPtr = 2;

	// Calculate AABB over primitives
	bounds = new AABB[count];
	centroids = new glm::vec3[count];

	for (int i = 0; i < count; i++)
	{
		bounds[i] = (*primitives)[i]->CalculateBounds();
		centroids[i] = (*primitives)[i]->Centroid();
	}

	//QuickSort(0, count - 1);
	
	// subdivide root node
	root->leftFirst = 0;
	root->count = count;
	for (int i = 0; i < count; i++)
	{
		indices[i];
		int a = 1;
	}
	CalculateBounds(root->leftFirst);
	Subdivide(root->leftFirst);

	ofstream saveFile;
	saveFile.open("BVH.bvh");
	for (int i = 0; i < count * 2; i++)
		saveFile << i << " :" << pool[i].leftFirst << " " << pool[i].count << endl;
	saveFile.close();
}
/*
void BVH::QuickSort(int left, int right)
{
	int l = left;
	int r = right;
	int tmp;
	int midIndex = indices[(l + r) / 2];
	int pivot = centroids[indices[midIndex]].x;

	while (l <= r) {
		while (centroids[indices[l]].x < pivot)
			l++;
		while (centroids[indices[r]].x > pivot)
			r--;
		if (l <= r) {
			tmp = indices[l];
			indices[l] = indices[r];
			indices[r] = tmp;
			l++;
			r--;
		}
	};

	if (left < r)
		QuickSort(left, r);
	if (l < right)
		QuickSort(l, right);
}
*/
void BVH::CalculateBounds(int node)
{
	int count = pool[node].count;

	if (count < 1)
		return;

	glm::vec3 mini;
	glm::vec3 maxi;

	int first = pool[node].leftFirst;

	AABB aabb = bounds[indices[first]];
	mini = aabb.pos1;
	maxi = aabb.pos2;
	
	
	for (int i = first + 1; i < first + count; i++)
	{
		aabb = bounds[indices[i]];
		maxi = glm::max(maxi, aabb.pos2);
		mini = glm::min(mini, aabb.pos1);
	}

	pool[node].corner1 = mini;
	pool[node].corner2 = maxi;
}

void BVH::Subdivide(int node)
{
	if (pool[node].count < 2)
		return;

	int left = poolPtr++;
	poolPtr++;

	int count = pool[node].count;
	int split = count / 2;

	pool[left].leftFirst = pool[node].leftFirst;
	pool[left+1].leftFirst = pool[node].leftFirst +split;

	pool[left].count = split;
	pool[left + 1].count = count - split;
	CalculateBounds(left);
	CalculateBounds(left + 1);
	pool[node].count = 0;
	pool[node].leftFirst = left;

	Subdivide(pool[node].leftFirst);
	Subdivide(pool[node].leftFirst + 1);
}

void BVH::Partition(int node)
{
	int count = pool[node].count;
	int split = count / 2;
	int lF = pool[node].leftFirst;

	pool[lF].count = split;
	pool[lF].leftFirst = 1;
	pool[lF+1].count = count - split;
	pool[lF+1].leftFirst = 1;
	CalculateBounds(lF);
	CalculateBounds(lF+1);
	pool[node].count = 0;
}

void BVH::Traverse(Ray & ray, int node)
{
	BVHNode* n = &pool[node];
	if (!n->Intersect(ray)) return;
	if (n->count)
	{
		IntersectPrimitives(ray, node);
	}
	else 
	{
		Traverse(ray, n->leftFirst);
		Traverse(ray, n->leftFirst + 1);
	}
}

void BVH::IntersectPrimitives(Ray & ray, int node)
{
	BVHNode* n = &pool[node];
	int first = n->leftFirst;
	int count = n->count;
	for (int i = first; i < first + count; i++)
		(*primitives)[indices[i]]->Intersect(ray);
}
