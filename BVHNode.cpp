#include "template.h"
#include "BVHNode.h"

float BVHNode::Area()
{
	glm::vec3 difference = corner2 - corner1;

	return difference.x * difference.y * difference.z;
}

float BVHNode::Cost()
{
	return Area() * count;
}