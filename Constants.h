#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cmath>

#define WINDOW_WIDTH 640 
#define WINDOW_HEIGHT 480
#define SPHERE_RADIUS (float)sqrt(0.25f*0.25f + 0.5f*0.5f)
#define PI 3.14159265359f


struct VERTEX
{
	float x, y, z;
	float r, g, b;
	//float nx, ny, nz;
	//float u, v;
};


struct TRIANGLE
{
	VERTEX v1, v2, v3;
};

struct TRIANGLE_NORMAL 
{
	VERTEX u1, u2;
	DirectX::XMVECTOR n;
};

struct CPU_VALUES 
{
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 proj;
};

struct LIGHT
{
	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT4 colour;
};
#endif