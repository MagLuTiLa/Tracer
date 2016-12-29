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

void BVH::QuickSort(int left, int right, int axis)
{
	if (!left < right)
		return;

	int l = left;
	int r = right;
	int tmp;
	int midIndex = indices[(l + r) / 2];
	int pivot = centroids[midIndex][axis];

	while (true)
	{
		while(centroids[indices[l]][axis] < pivot)
			l++;
		while(centroids[indices[r]][axis] > pivot)
			r--;
		if (l >= r)
			break;
		tmp = indices[l];
		indices[l] = indices[r];
		indices[r] = tmp;
	}

	QuickSort(left, r, axis);
	QuickSort(r+1, right, axis);
}

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
	int lf = pool[node].leftFirst;
	int count = pool[node].count;
	QuickSort(lf, lf + count);
	int split = count / 2;

	pool[left].leftFirst = lf;
	pool[left+1].leftFirst = lf +split;

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

void BVH::Traverse(Ray & ray, int node, int* depth)
{
	if(depth != NULL)
		depth[0]++;
	if (node == 27)
		int a = 0;
	if (node == 28)
		int b = 0;
	if (node == 30)
		int c = 0;
	BVHNode* n = &pool[node];
	if (n->count)
	{
		IntersectPrimitives(ray, node);
	}
	else 
	{
		float dist[2];
		bool traverse[2];
		traverse[0] = pool[n->leftFirst].Intersect(ray, dist[0]);
		traverse[1] = pool[n->leftFirst + 1].Intersect(ray, dist[1]);
		bool first = dist[0] > dist[1];
		if(traverse[first])
			Traverse(ray, n->leftFirst + first, depth);
		if(traverse[first^1] & ray.length > dist[first ^ 1])
			Traverse(ray, n->leftFirst + first^1, depth);
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
