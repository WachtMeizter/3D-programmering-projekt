#include "Sphere.h"

Sphere::Sphere()
{
	this->tCap = 200;
	this->triangle = new TRIANGLE*[this->tCap];
	for (int i = 0; i < 200; i++)
	{
		this->triangle[i] = nullptr;
	}
}

Sphere::~Sphere()
{
	for (int i = 0; i < this->nrOfTriangles; i++)
	{
		delete this->triangle[i];
	}
	delete[] this->triangle;
}

Sphere Sphere::operator=(const Sphere & copyObj)
{
	for (int i = 0; i < copyObj.nrOfTriangles; i++)
	{
		this->AddTriangle(copyObj.triangle[i]->v1, copyObj.triangle[i]->v2, copyObj.triangle[i]->v3);
	}
	this->tCap = copyObj.tCap;

	return *this;
}


void Sphere::AddTriangle(VERTEX v1, VERTEX v2, VERTEX v3)
{
	if (this->nrOfTriangles == this->tCap)
		this->ExpandTCap();
	this->triangle[this->nrOfTriangles++] = new TRIANGLE{ v1, v2, v3 };
}

void Sphere::CreateIsosphere(float x, float y, float z, int refine)
{
	VERTEX v[] = 
	{
		-0.25f + x, 0.5f + y, 0.0f + z, 0.2f, 0.2f, 0.2f,
		0.25f + x, 0.5f + y, 0.0f + z, 0.2f, 0.2f, 0.2f,
		-0.25f + x, -0.5f + y, 0.0f + z, 0.2f, 0.2f, 0.2f,
		0.25f + x, -0.5f + y, 0.0f + z, 0.2f, 0.2f, 0.2f,
		0.0f + x, -0.25f + y, -0.5f + z, 0.2f, 0.2f, 0.2f,
		0.0f + x, 0.25f + y, -0.5f + z,0.2f, 0.2f, 0.2f,
		0.0f + x, -0.25f + y, 0.5f + z, 0.2f, 0.2f, 0.2f,
		0.0f + x, 0.25f + y, 0.5f + z, 0.2f, 0.2f, 0.2f,
		0.5f + x, 0.0f + y, 0.25f + z,0.2f, 0.2f, 0.2f,
		0.5f + x, 0.0f + y, -0.25f + z, 0.2f, 0.2f, 0.2f,
		-0.5f + x, 0.0f + y, 0.25f + z, 0.2f, 0.2f, 0.2f,
		-0.5f + x, 0.0f + y, -0.25f + z, 0.2f, 0.2f, 0.2f,
	};

	this->AddTriangle(v[0], v[11], v[5]);
	this->AddTriangle(v[0], v[5], v[1]);
	this->AddTriangle(v[0], v[1], v[7]);
	this->AddTriangle(v[0], v[7], v[10]);
	this->AddTriangle(v[0], v[10], v[11]);
	this->AddTriangle(v[1], v[5], v[9]);
	this->AddTriangle(v[5], v[11], v[4]);
	this->AddTriangle(v[11], v[10], v[2]);
	this->AddTriangle(v[10], v[7], v[6]);
	this->AddTriangle(v[7], v[1], v[8]);
	this->AddTriangle(v[3], v[9], v[4]);
	this->AddTriangle(v[3], v[4], v[2]);
	this->AddTriangle(v[3], v[2], v[6]);
	this->AddTriangle(v[3], v[6], v[8]);
	this->AddTriangle(v[3], v[8], v[9]);
	this->AddTriangle(v[4], v[9], v[5]);
	this->AddTriangle(v[2], v[4], v[11]);
	this->AddTriangle(v[6], v[2], v[10]);
	this->AddTriangle(v[8], v[6], v[7]);
	this->AddTriangle(v[9], v[8], v[1]);

	for (int i = 0; i < refine; i++)
		this->refineTriangle();
}

void Sphere::refineTriangle()
{
	this->tCap *= 4;
	TRIANGLE **temp = new TRIANGLE*[this->tCap];

	TRIANGLE midPoints;

	for (int i = 0; i < this->nrOfTriangles; i++)
	{
		midPoints			= this->GetMiddlePoint(*this->triangle[i]);
		temp[i * 4]			= new TRIANGLE{ this->triangle[i]->v1, midPoints.v1, midPoints.v3 };
		temp[(i * 4) + 1]	= new TRIANGLE{ this->triangle[i]->v2, midPoints.v2, midPoints.v1 };
		temp[(i * 4) + 2]	= new TRIANGLE{ this->triangle[i]->v3, midPoints.v3, midPoints.v2 };
		temp[(i * 4) + 3]	= new TRIANGLE{ midPoints.v1, midPoints.v2, midPoints.v3 };
	}

	delete[] this->triangle;
	this->triangle = temp;

	this->nrOfTriangles *= 4;
}

TRIANGLE Sphere::GetTriangleList(int index)const
{
	return *this->triangle[index];
}

TRIANGLE Sphere::GetMiddlePoint(TRIANGLE triangle) const
{
	VERTEX v12, v13, v23;
	v12.x = (triangle.v1.x + triangle.v2.x) / 2.0f;
	v13.x = (triangle.v1.x + triangle.v3.x) / 2.0f;
	v23.x = (triangle.v2.x + triangle.v3.x) / 2.0f;
	v12.y = (triangle.v1.y + triangle.v2.y) / 2.0f;
	v13.y = (triangle.v1.y + triangle.v3.y) / 2.0f;
	v23.y = (triangle.v2.y + triangle.v3.y) / 2.0f;
	v12.z = (triangle.v1.z + triangle.v2.z) / 2.0f;
	v13.z = (triangle.v1.z + triangle.v3.z) / 2.0f;
	v23.z = (triangle.v2.z + triangle.v3.z) / 2.0f;

	float vertexToCentre = sqrt(v12.x * v12.x + v12.y * v12.y + v12.z * v12.z);
	v12 = { v12.x * (SPHERE_RADIUS / vertexToCentre),
		v12.y * (SPHERE_RADIUS / vertexToCentre),
		v12.z * (SPHERE_RADIUS / vertexToCentre),
		.2f, 0.2f, 0.2f };

	vertexToCentre = sqrt(v23.x * v23.x + v23.y * v23.y + v23.z * v23.z);
	v23 = { v23.x * (SPHERE_RADIUS / vertexToCentre),
		v23.y * (SPHERE_RADIUS / vertexToCentre),
		v23.z * (SPHERE_RADIUS / vertexToCentre),
		0.5f, 0.2f, 0.2f };

	vertexToCentre = sqrt(v13.x * v13.x + v13.y * v13.y + v13.z * v13.z);
	v13 = { v13.x * (SPHERE_RADIUS / vertexToCentre),
		v13.y * (SPHERE_RADIUS / vertexToCentre),
		v13.z * (SPHERE_RADIUS / vertexToCentre),
		1.f, 0.2f, .2f };


	return { v12, v23, v13 };
}

DirectX::XMVECTOR Sphere::GetNormalAt(int index) const
{
	TRIANGLE tempTri;
	DirectX::XMVECTOR vec1, vec2;
	tempTri = this->GetTriangleList(index); 
	
	vec1 = DirectX::XMLoadFloat3(&addVector(tempTri.v1, tempTri.v3));
	vec2 = DirectX::XMLoadFloat3(&addVector(tempTri.v1, tempTri.v2));
	

	return DirectX::XMVector3Cross(vec1, vec2);
}

int Sphere::GetNrOfVertices() const
{
	return this->nrOfTriangles * 3;
}

int Sphere::GetNrOfTriangles() const
{
	return this->nrOfTriangles;
}

void Sphere::ExpandTCap()
{
	this->tCap *= 2;
	TRIANGLE** temp = new TRIANGLE*[this->tCap];
	for (int i = 0; i < this->nrOfTriangles; i++)
	{
		temp[i] = this->triangle[i];
	}
	for (int i = this->nrOfTriangles; i < this->tCap; i++)
	{
		temp[i] = nullptr;
	}
	delete[] this->triangle;
	this->triangle = temp;
}

DirectX::XMFLOAT3 Sphere::addVector(VERTEX pointB, VERTEX pointA) const
{
	DirectX::XMFLOAT3 rtnVec;

	rtnVec = DirectX::XMFLOAT3(pointB.x - pointA.x, pointB.y - pointA.y, pointB.z - pointA.z);

	return rtnVec;
}
