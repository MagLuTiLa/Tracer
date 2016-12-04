#pragma once
#include "mesh.h"
#include "triangle.h"
class TriangleMesh :
	public Mesh
{
public:
	TriangleMesh(Triangle* triangles, unsigned int size);
	TriangleMesh();
	~TriangleMesh();
	Triangle* triangles;
	unsigned int Size();
	unsigned int size;
};

