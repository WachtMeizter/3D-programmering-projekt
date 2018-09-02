#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include "Constants.h"
#include "Sphere.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#ifndef D3DFUNCTIONS_H
#define D3DFUNCTIONS_H

namespace
{	
	//Global variables
	DirectX::XMMATRIX gMatrix;
	DirectX::XMFLOAT4X4 proj;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 world;
	//Global D3D-pointers
	ID3D11DeviceContext		*gDeviceContext = nullptr;
	IDXGISwapChain			*gSwapChain = nullptr;
	ID3D11Device			*gDevice = nullptr;
	ID3D11RenderTargetView	*gBackBuffer = nullptr;
	ID3D11InputLayout		*gLayout;
	ID3D11VertexShader		*gVertexShader = nullptr;
	ID3D11PixelShader		*gPixelShader = nullptr;
	ID3D11GeometryShader	*gGeometryShader = nullptr;
	ID3D11Buffer			*gVertexBuffer = nullptr;
	ID3D11Buffer			*gConstantBuffer = nullptr;
}

//Window
HWND InitWindow(HINSTANCE hInstance);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
// Initiate DirectX3D
HRESULT InitD3D(HWND hWnd);
HRESULT CreateShaders();
void InitMatrices();
void SetViewPort(HWND hWnd);
// 3D functions
void CreateSphere();
void CreateConstantBuffer();
void Render();

// Release COM objects
void Clean();


#endif