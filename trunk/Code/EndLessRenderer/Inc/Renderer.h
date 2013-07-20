//Renderer.h	∂®“Â‰÷»æ∆˜
#pragma once

#include "EndlessRenderer.h"
#include <d3d11.h>

class ERenderObj;
class ELightBase;
class ECameraBase;

class ERenderer : public EEndlessRenderer
{
//Render Public API
public:
	virtual void Init(HWND);

	virtual void Update();
	virtual void Render();

	virtual UINT32 CreateRenderObj();
	virtual bool   DeleteRenderObj(UINT32);
	virtual EEndlessRenderObj* GetRenderObj(UINT32);

	virtual UINT32 CreateLight();
	virtual bool   DeleteLight(UINT32);
	virtual EEndlessLight* GetLight(UINT32);

	//Only support single camera
	virtual EEndlessCamera* GetCamera();

public:
	ERenderer();
	virtual ~ERenderer();

private:
	void _InitDevice(HWND);
	void _CreateGlobalD3DResource();
	void _ReleaseGlobalD3DResource();
	void _ReleaseAllObjects();

private:
// RenderObj,Light and Camera
	UINT32	m_RenderObjId;
	UINT32	m_LightId;
	typedef std::map<UINT32, ERenderObj*> RenderObjMap;
	RenderObjMap m_RenderObjMap;
	typedef std::map<UINT32, ELightBase*> LightMap;
	LightMap m_LightMap;
	ECameraBase* m_pCamera;
};

// global d3d device
extern ID3D11Device*		g_pDevice;
extern ID3D11DeviceContext* g_pDeviceContext;
extern IDXGISwapChain*      g_pSwapChain;

// global d3d resource
ID3D11RenderTargetView*		g_pBackBufferView;
ID3D11DepthStencilView*		g_pDepthView;		//DXGI_FORMAT_D32_FLOAT