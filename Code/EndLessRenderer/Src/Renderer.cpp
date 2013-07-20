//Renderer.cpp:	äÖÈ¾Æ÷ÊµÏÖ

#include "Renderer.h"
#include "CameraBase.h"
#include "RenderObj.h"
#include "LightBase.h"

EEndlessRenderer* g_pRenderer = new ERenderer;

ID3D11Device*			g_pDevice = 0;
ID3D11DeviceContext*    g_pDeviceContext = 0;
IDXGISwapChain*         g_pSwapChain = 0;

ERenderer::ERenderer()
{
	m_RenderObjId = 0;
	m_LightId = 0;
	m_RenderObjMap.clear();
	m_LightMap.clear();
	m_pCamera = 0;
}

ERenderer::~ERenderer()
{
	//Release all objects in renderer
	_ReleaseAllObjects();

	//Release D3D Global Resource
	_ReleaseGlobalD3DResource();

	//Release Device
	if(g_pSwapChain)
	{
		g_pSwapChain->Release();
		g_pSwapChain = 0;
	}
	if(g_pDeviceContext)
	{
		g_pDeviceContext->Release();
		g_pDeviceContext = 0;
	}
	if(g_pSwapChain)
	{
		g_pSwapChain->Release();
		g_pSwapChain = 0;
	}
}

void ERenderer::Init(HWND hWND)
{
	//Create D3D Device
	_InitDevice(hWND);

	//Create Default Resource
	_CreateGlobalD3DResource();
}

void ERenderer::Update()
{

}

void ERenderer::Render()
{

}

UINT32 ERenderer::CreateRenderObj()
{
	m_RenderObjId++;

	ERenderObj* pRenderObj = new ERenderObj;
	pRenderObj->m_Id = m_RenderObjId;

	m_RenderObjMap.insert(std::pair<UINT32, ERenderObj*>(m_RenderObjId, pRenderObj));

	return m_RenderObjId;
}

bool ERenderer::DeleteRenderObj(UINT32 Id)
{
	RenderObjMap::iterator it = m_RenderObjMap.find(Id);
	
	if(it != m_RenderObjMap.end())
	{
		ERenderObj* pRenderObj = it->second;
		if(pRenderObj)
			delete pRenderObj;
		m_RenderObjMap.erase(it);

		return true;
	}

	return false;
}

EEndlessRenderObj* ERenderer::GetRenderObj(UINT32 Id)
{
	RenderObjMap::iterator it = m_RenderObjMap.find(Id);

	if(it != m_RenderObjMap.end())
	{
		return it->second;
	}

	return 0;
}

UINT32 ERenderer::CreateLight()
{
	m_LightId++;

	ELightBase* pLight = new ELightBase;
	pLight->m_Id = m_LightId;

	m_LightMap.insert(std::pair<UINT32, ELightBase*>(m_LightId, pLight));

	return m_LightId;
}

bool ERenderer::DeleteLight(UINT32 Id)
{
	LightMap::iterator it = m_LightMap.find(Id);

	if(it != m_LightMap.end())
	{
		ELightBase* pLight = it->second;
		if(pLight)
			delete pLight;
		m_LightMap.erase(it);

		return true;
	}

	return false;
}

EEndlessLight* ERenderer::GetLight(UINT32 Id)
{
	LightMap::iterator it = m_LightMap.find(Id);

	if(it != m_LightMap.end())
	{
		return it->second;
	}

	return 0;
}

EEndlessCamera* ERenderer::GetCamera()
{
	if(!m_pCamera)
		m_pCamera = new ECameraBase;

	return m_pCamera;
}

void ERenderer::_InitDevice(HWND hWnd)
{
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect( hWnd, &rc );

	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels[] = {D3D_FEATURE_LEVEL_11_0};
	D3D_FEATURE_LEVEL featureLevel;

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof( sd ) );
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	hr = D3D11CreateDeviceAndSwapChain( NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevels, 1,
		D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pDevice, &featureLevel, &g_pDeviceContext );

	assert(hr == S_OK);
}

void ERenderer::_CreateGlobalD3DResource()
{
	//Create Back Buffer View
	ID3D11Texture2D* pRT= 0;
	g_pSwapChain->GetBuffer(0, __uuidof(pRT), reinterpret_cast<void**>(&pRT));

	g_pBackBufferView = 0;
	HRESULT hResult = g_pDevice->CreateRenderTargetView(pRT, 0, &g_pBackBufferView);

	assert(hResult == S_OK);

	//Create Depth Buffer
	ID3D11Texture2D* pDepthTex = 0;
	D3D11_TEXTURE2D_DESC TexDesc;
	pRT->GetDesc(&TexDesc);
	TexDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	TexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	hResult = g_pDevice->CreateTexture2D(&TexDesc ,0 , &pDepthTex);

	//Create Depth Buffer View
	D3D11_DEPTH_STENCIL_VIEW_DESC DepthViewDesc;
	DepthViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	DepthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DepthViewDesc.Texture2D.MipSlice= 0;
	DepthViewDesc.Flags = 0;

	g_pDepthView = 0;
	hResult = g_pDevice->CreateDepthStencilView(pDepthTex, &DepthViewDesc, &g_pDepthView);

	assert(hResult == S_OK);
}

void ERenderer::_ReleaseGlobalD3DResource()
{
	//Release Back Buffer Texture and View
	if(g_pBackBufferView)
	{
		ID3D11Texture2D* pBackBuffer = 0;
		g_pBackBufferView->GetResource((ID3D11Resource**)&pBackBuffer);
		g_pBackBufferView->Release();
		if(pBackBuffer)
			pBackBuffer->Release();
	}
	//Release Depth Buffer Texture and View 
	if(g_pDepthView)
	{
		ID3D11Texture2D* pDepthBuffer = 0;
		g_pDepthView->GetResource((ID3D11Resource**)&pDepthBuffer);
		g_pDepthView->Release();
		if(pDepthBuffer)
			pDepthBuffer->Release();
	}
}

void ERenderer::_ReleaseAllObjects()
{
	for(RenderObjMap::iterator it = m_RenderObjMap.begin(); it != m_RenderObjMap.end(); ++it)
	{
		EEndlessRenderObj* pRenderObj = it->second;
		if(pRenderObj)	
			delete pRenderObj;
	}

	for(LightMap::iterator it = m_LightMap.begin(); it != m_LightMap.end(); ++it)
	{
		EEndlessLight* pLight = it->second;
		if(pLight)	
			delete pLight;
	}
	
	if(m_pCamera)	
	{
		delete m_pCamera;
		m_pCamera = 0;
	}

	m_RenderObjId = 0;
	m_LightId = 0;
	m_RenderObjMap.clear();
	m_LightMap.clear();
}
