#pragma once
struct BVHNode
{
	BVHNode(AABB aabb) :
		bounds(aabb)
	{}

	AABB bounds;
	int leftFirst;
	int count;
};

struct AABB
{
	AABB(glm::vec3 p1, glm::vec3 p2) :
		pos1(p1),
		pos2(p2)
	{}

	glm::vec3 pos1;
	glm::vec3 pos2;
};