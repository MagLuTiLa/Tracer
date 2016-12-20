#pragma once

#include <vector>

struct AABB
{
	AABB() :
		pos1(glm::vec3()),
		pos2(glm::vec3())
	{}

	AABB(glm::vec3 p1, glm::vec3 p2) :
		pos1(p1),
		pos2(p2)
	{}

	glm::vec3 pos1;
	glm::vec3 pos2;
};

struct BVHNode
{
	BVHNode() :
		corner1(glm::vec3()),
		corner2(glm::vec3())
	{}

	BVHNode(AABB aabb) :
		corner1(aabb.pos1),
		corner2(aabb.pos2)
	{}

	union 
	{
		struct
		{
			glm::vec3 corner1;
			int leftFirst;
		};
		__m128 c1;
	};

	union
	{
		struct
		{
			glm::vec3 corner2;
			int count;
		};
		__m128 c2;
	};

	float Area();
	float Cost();
	bool Intersect(Ray&);
};
/*
class BVHTree
{
	BVHTree()
	{
	};

	vector<Primitive*> yay;
};*/