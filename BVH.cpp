#include "template.h"
#include "BVH.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>


BVH::BVH()
{
}

void BVH::ConstructBVH(std::vector<Primitive*>* p)
{
	primitives = p;
	int count = primitives->size();
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

	QuickSort(0, count - 1, 0);
	
	// subdivide root node
	root->leftFirst = 0;
	root->count = count;

	CalculateBounds(root->leftFirst);
	Subdivide(root->leftFirst);

	WriteToFile();
}

void BVH::ConstructBVHSAH(std::vector<Primitive*>* p)
{
	primitives = p;
	int count = primitives->size();
	// create index array
	indices = new int[count];
	for (int i = 0; i < count; i++)
		indices[i] = i;
	// allocate BVH root node
	pool = reinterpret_cast<BVHNode*>(_mm_malloc(count * 2 * sizeof(BVHNode), 64));// [count * 2];
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

	// subdivide root node
	root->leftFirst = 0;
	root->count = count;

	CalculateBounds(root->leftFirst);

	SubdivideSAH(root->leftFirst);

	WriteToFile();
}

void BVH::QuickSort(int left, int right, int axis)
{
	if (!left < right)
		return;

	int l = left;
	int r = right;
	int tmp;
	int midIndex = indices[(l + r) / 2];
	float pivot = centroids[midIndex][axis];

	while (true)
	{
		float value = centroids[indices[l]][axis];
		while (value < pivot)
		{
			l++;
			value = centroids[indices[l]][axis];
		}

		value = centroids[indices[r]][axis];
		while (centroids[indices[r]][axis] > pivot)
		{
			r--;
			value = centroids[indices[r]][axis];
		}

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

void BVH::SubdivideSAH(int node)
{
	if (pool[node].count < 2)
		return;

	int left = poolPtr++;
	poolPtr++;
	int lf = pool[node].leftFirst;
	int count = pool[node].count;

	glm::vec3 axisSize = pool[node].corner2 - pool[node].corner1;

	int maxAxis = 0;

	if (axisSize[1] > axisSize[maxAxis])
		maxAxis = 1;
	if (axisSize[2] > axisSize[maxAxis])
		maxAxis = 2;

	QuickSort(lf, lf + count -1, maxAxis);

	int bestCost = pool[node].Cost();
	int bestSplit = 0;

	int split = 1;
	for (split; split < count; split++)
	{
		pool[left].leftFirst = lf;
		pool[left + 1].leftFirst = lf + split;

		pool[left].count = split;
		pool[left + 1].count = count - split;

		CalculateBounds(left);
		CalculateBounds(left + 1);

		int costLeft = pool[left].Cost();
		int costRight = pool[left+1].Cost();

		int newCost = costLeft + costRight;

		if (costLeft + costRight < bestCost)
		{
			bestCost = newCost;
			bestSplit = split;
		}
	}

	if (bestSplit != 0)
	{
		if (bestSplit != split)
		{
			pool[left].leftFirst = lf;
			pool[left + 1].leftFirst = lf + bestSplit;

			pool[left].count = bestSplit;
			pool[left + 1].count = count - bestSplit;

			CalculateBounds(left);
			CalculateBounds(left + 1);
		}

		pool[node].count = 0;
		pool[node].leftFirst = left;

		SubdivideSAH(pool[node].leftFirst);
		SubdivideSAH(pool[node].leftFirst + 1);
	}
}

void BVH::WriteToFile()
{
	ofstream saveFile;
	saveFile.open("BVH.bvh");
	for (int i = 0; i < poolPtr; i++)
		saveFile << i << " :" << pool[i].leftFirst << " " << pool[i].count << endl;
	saveFile.close();
}

void BVH::Traverse(Ray & ray, int node, int* depth)
{
	if(depth != NULL)
		depth[0]++;

	BVHNode* n = &pool[node];

	if (!n->Intersect(ray)) return;
	if (n->count)
	{
		IntersectPrimitives(ray, node);
	}
	else 
	{
		Traverse(ray, n->leftFirst, depth);
		Traverse(ray, n->leftFirst + 1, depth);
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
