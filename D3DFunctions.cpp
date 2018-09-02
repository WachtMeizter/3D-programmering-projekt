#include "D3DFunctions.h"

Sphere sphere;
CPU_VALUES valuesFromCpu = {  };
float rot;

HRESULT CreateShaders(void)
{
	ID3DBlob 
		*pVS = nullptr,
		*pPS = nullptr,
		*pErr = nullptr;

	//Create Vertex Shader
	HRESULT result = D3DCompileFromFile(L"VertexShader.hlsl", nullptr, nullptr, "VS_main", "vs_5_0", 0, 0, &pVS, &pErr);

	if (FAILED(result))
	{
		if (pErr)
		{
			OutputDebugStringA((char*)pErr->GetBufferPointer());
			pErr->Release();
		}
		if (pVS)
			pVS->Release();
		return result;
	}

	gDevice->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &gVertexShader);
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			12,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
	};
	gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &gLayout);

	pVS->Release();

	//Release Error blob if it's being used
	if (pErr) pErr->Release();
	//Create Fragment Shader
	result = D3DCompileFromFile(L"FragmentShader.hlsl", 0, 0, "PS_main", "ps_5_0", 0, 0, &pPS, &pErr);

	if (result)
	{
		if (pErr)
		{
			OutputDebugStringA((char*)pErr->GetBufferPointer());
			pErr->Release();
		}
		if (pPS)
			pPS->Release();
		return result;
	}
	gDevice->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &gPixelShader);
	pPS->Release();

	ID3DBlob* pGS = nullptr;
	D3DCompileFromFile(
		L"GeometryShader.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"GS_main",		// entry point
		"gs_5_0",		// shader model (target)
		0,				// shader compile options
		0,				// effect compile options
		&pGS,			// double pointer to ID3DBlob		
		nullptr			// pointer for Error Blob messages.
	);
	gDevice->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(), nullptr, &gGeometryShader);
	
	pGS->Release();

	return S_OK;
}

void InitMatrices()
{
	//view
	DirectX::XMStoreFloat4x4(&valuesFromCpu.view, DirectX::XMMatrixIdentity());
	
	DirectX::XMVECTOR camPos = { 0.0f, 0.0f, -2.0f, 0.0f };
	DirectX::XMVECTOR camLook = { 0.0f, 0.0f, 0.0f, 0.0f };
	DirectX::XMVECTOR camUp = { 0.0f, 1.0f, 0.0f, 1.0f };
	
	DirectX::XMStoreFloat4x4(&valuesFromCpu.view, DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtLH(camPos, camLook, camUp)));
	//world

	DirectX::XMStoreFloat4x4(&valuesFromCpu.world, DirectX::XMMatrixIdentity());
	//projection

	DirectX::XMStoreFloat4x4(&valuesFromCpu.proj, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&valuesFromCpu.proj, DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovLH((PI * 0.45f), (float(WINDOW_WIDTH) / (float)WINDOW_HEIGHT), 0.1f, 20.0f)));
	
}

//void CreateTriangles()
//{
//	VERTEX triangles[] =
//	{
//
//		0.0f, 0.5f, 0.0f,
//		1.0f, 0.0f, 0.0f,
//
//		0.5f, -0.5f, 0.0f,
//		0.0f, 1.0f, 0.0f,
//
//		-0.5f, -0.5f, 0.0f,
//		0.0f, 0.0f, 1.0f
//	};
//
//	D3D11_BUFFER_DESC bufferDesc;
//	memset(&bufferDesc, 0, sizeof(bufferDesc));
//
//	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	bufferDesc.ByteWidth = sizeof(triangles);
//
//	D3D11_SUBRESOURCE_DATA data;
//	data.pSysMem = triangles;
//
//	gDevice->CreateBuffer(&bufferDesc, &data, &gVertexBuffer);
//}

void Render()
{
	float backgroundRGB[] = { 0, 0, 0, 1 };
	
	gDeviceContext->ClearRenderTargetView(gBackBuffer, backgroundRGB);

	gDeviceContext->VSSetShader(gVertexShader, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->GSSetShader(gGeometryShader, nullptr, 0);
	gDeviceContext->PSSetShader(gPixelShader, nullptr, 0);

	UINT32 vertexSize = sizeof(VERTEX);
	UINT32 offset = 0;

	gDeviceContext->IASetVertexBuffers(0, 1, &gVertexBuffer, &vertexSize, &offset);
	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gDeviceContext->IASetInputLayout(gLayout);

	D3D11_MAPPED_SUBRESOURCE dataPtr;
	gDeviceContext->Map(gConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
	// copy memory from CPU to GPU the entire struct
	rot += 0.0002f;
	rot /= 1.0f;
	DirectX::XMStoreFloat4x4(&valuesFromCpu.world, DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationY(rot)));
	memcpy(dataPtr.pData, &valuesFromCpu, sizeof(valuesFromCpu));
	// UnMap constant buffer so that we can use it again in the GPU
	gDeviceContext->Unmap(gConstantBuffer, 0);
	// set resource to Vertex Shader
	gDeviceContext->VSSetConstantBuffers(0, 1, &gConstantBuffer);
	gDeviceContext->GSSetConstantBuffers(0, 1, &gConstantBuffer);

	gDeviceContext->Draw(sphere.GetNrOfVertices(), 0);

	gSwapChain->Present(0, 0);
}

void Clean()
{
	gLayout->Release();
	gVertexBuffer->Release();
	gVertexShader->Release();
	gPixelShader->Release();
	gSwapChain->Release();
	gBackBuffer->Release();
	gDevice->Release();
	gDeviceContext->Release();
	gConstantBuffer->Release();
}

void CreateSphere()
{ 

	sphere.CreateIsosphere(0.0f, 0.0f, 0.0f, 5); //
	TRIANGLE *triangles = new TRIANGLE[sphere.GetNrOfTriangles()];

	for (int i = 0; i < sphere.GetNrOfTriangles(); i++)
		triangles[i] = sphere.GetTriangleList(i);

	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));

	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(TRIANGLE) * sphere.GetNrOfTriangles();

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = triangles;
	
	HRESULT result = gDevice->CreateBuffer(&bufferDesc, &data, &gVertexBuffer);
	delete triangles;
	if (FAILED(result))
	{
		exit(-1);
	}
	
	
	
}

void CreateConstantBuffer()
{
	
	D3D11_BUFFER_DESC cBuffDesc;
	cBuffDesc.Usage = D3D11_USAGE_DYNAMIC;
	cBuffDesc.ByteWidth = sizeof(CPU_VALUES);
	cBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cBuffDesc.MiscFlags = 0;
	cBuffDesc.StructureByteStride = 0;

	// check if the creation failed for any reason
	HRESULT hr = 0;
	hr = gDevice->CreateBuffer(&cBuffDesc, nullptr, &gConstantBuffer);
	if (FAILED(hr))
	{
		// handle the error, could be fatal or a warning...
		exit(-1);
	}


}


void SetViewPort(HWND hWnd)
{
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)WINDOW_WIDTH;
	viewport.Height = (float)WINDOW_HEIGHT;
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.0f;

	gDeviceContext->RSSetViewports(1, &viewport);
}

HRESULT InitD3D(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC desc;

	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));

	desc.BufferCount = 1;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = hWnd;
	desc.SampleDesc.Count = 4;
	desc.Windowed = TRUE;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&desc,
		&gSwapChain,
		&gDevice,
		NULL,
		&gDeviceContext);

	if (SUCCEEDED(hr))
	{

		ID3D11Texture2D *pBackBuffer;
		gSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		gDevice->CreateRenderTargetView(pBackBuffer, NULL, &gBackBuffer);
		pBackBuffer->Release();

		gDeviceContext->OMSetRenderTargets(1, &gBackBuffer, NULL);
	}


	return hr;
}
