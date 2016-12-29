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

bool BVHNode::Intersect(Ray & ray)
{
	/*
	__m128 t1 = _mm_mul_ps(_mm_sub_ps(c1, ray.quadOrigin), ray.quadDirection);
	__m128 t2 = _mm_mul_ps(_mm_sub_ps(c2, ray.quadOrigin), ray.quadDirection);
	*/

	//TODO Instead of dividing by quaddirection, multiply by the reciprocal of quaddirecction. Have to adjust ray class for that 
	__m128 t1 = _mm_div_ps(_mm_sub_ps(c1, ray.quadOrigin), ray.quadDirection);
	__m128 t2 = _mm_div_ps(_mm_sub_ps(c2, ray.quadOrigin), ray.quadDirection);
	__m128 vmax4 = _mm_max_ps(t1, t2), vmin4 = _mm_min_ps(t1, t2);

	float* vmax = (float*)&vmax4, *vmin = (float*)&vmin4;
	float tmax = min(vmax[0], min(vmax[1], vmax[2]));
	float tmin = max(vmin[0], max(vmin[1], vmin[2]));

	return (tmax >= tmin && tmax >= 0);
	
	/*
	float tx1 = (corner1.x - ray.origin.x) / ray.direction.x;
	float tx2 = (corner2.x - ray.origin.x) / ray.direction.x;
	float tmin = min(tx1, tx2);
	float tmax = max(tx1, tx2);

	float ty1 = (corner1.y - ray.origin.y) / ray.direction.y;
	float ty2 = (corner2.y - ray.origin.y) / ray.direction.y;
	tmin = max(tmin, min(ty1, ty2));
	tmax = min(tmax, max(ty1, ty2));

	float tz1 = (corner1.z - ray.origin.z) / ray.direction.z;
	float tz2 = (corner2.z - ray.origin.z) / ray.direction.z;
	tmin = max(tmin, min(tz1, tz2));
	tmax = min(tmax, max(tz1, tz2));

	return (tmax >= tmin && tmax >= 0);
	*/
}

