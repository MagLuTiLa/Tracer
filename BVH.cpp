#include "template.h"
#include "BVH.h"


BVH::BVH()
{
}

void BVH::ConstructBVH(std::vector<Primitive*> primitives, int count)
{
	// create index array
	indices = new int[count];
	for (int i = 0; i < count; i++) 
		indices[i] = i;
	// allocate BVH root node
	pool = new BVHNode[count * 2 - 1];
	BVHNode* root = &pool[0];
	poolPtr = 2;

	// Calculate AABB over primitives
	bounds = new AABB[count];
	centroids = new glm::vec3[count];

	for (int i = 0; i < count; i++)
	{
		bounds[i] = primitives[i]->CalculateBounds();
		centroids[i] = primitives[i]->Centroid();
	}

	QuickSort(0, count - 1);
	
	// subdivide root node
	root->leftFirst = 0;
	root->count = count;
	for (int i = 0; i < count; i++)
	{
		indices[i];
		int a = 1;
	}
	CalculateBounds(root->leftFirst);
	//Subdivide(root->leftFirst);
}

void BVH::QuickSort(int left, int right)
{
	if (!left < right)
		return;

	int l = left;
	int r = right;
	int tmp;
	int midIndex = indices[(l + r) / 2];
	int pivot = centroids[indices[midIndex]].x;

	while (true)
	{
		while(centroids[indices[l]].x < pivot)
			l++;
		while(centroids[indices[r]].x > pivot)
			r--;
		if (l >= r)
			break;
		tmp = indices[l];
		indices[l] = indices[r];
		indices[r] = tmp;
	}

	QuickSort(left, r);
	QuickSort(r+1, right);
}

void BVH::CalculateBounds(int node)
{
	int count = pool[node].count;

	if (count < 1)
		return;

	glm::vec3 min;
	glm::vec3 max;

	AABB aabb = bounds[indices[node]];
	min = aabb.pos1;
	max = aabb.pos2;
	
	for (int i = node + 1; i < node + count; i++)
	{
		aabb = bounds[indices[i]];
		max = glm::max(max, aabb.pos2);
		min = glm::min(min, aabb.pos1);
	}

	pool[node].corner1 = min;
	pool[node].corner2 = max;
}

void BVH::Subdivide(int node)
{
	if (pool[node].count < 2)
		return;

	pool[node].leftFirst = poolPtr++;
	poolPtr++;
	Partition(node);
	Subdivide(pool[node].leftFirst);
	Subdivide(pool[node].leftFirst + 1);
}

void BVH::Partition(int node)
{
	int count = pool[node].count;

	int split = count / 2 - 1;

	vec3 mid = (pool[node].corner1 + pool[node].corner2) / 2.f;

	int leftFirst = pool[node].leftFirst;

	for (int i = leftFirst; i < leftFirst + count -1; i++)
	{
		glm::vec3 cent = centroids[indices[i]];
		if (cent.x <= mid.x)
		{
		}
	}
}