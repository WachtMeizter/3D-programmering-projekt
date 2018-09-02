#ifndef SPHERE_H
#define SPHERE_H

#include "D3DFunctions.h"

class Sphere
{
public:
	Sphere();
	~Sphere();
	Sphere operator=(const Sphere &copyObj);
	void AddTriangle(VERTEX v1, VERTEX v2, VERTEX v3);
	void CreateIsosphere(float x, float y, float z, int refine = 0);/*float x, float y, float z,*/
	void refineTriangle();

	TRIANGLE GetTriangleList(int index)const;
	TRIANGLE GetMiddlePoint(TRIANGLE triangle)const;
	DirectX::XMVECTOR GetNormalAt(int index)const;
	int GetNrOfVertices()const;
	int GetNrOfTriangles()const;
	void ExpandTCap();
private:
	TRIANGLE **triangle;
	int nrOfTriangles;
	int tCap;
	DirectX::XMFLOAT3 addVector(VERTEX v1, VERTEX v2) const;
};
#endif // !SPHERE_H
