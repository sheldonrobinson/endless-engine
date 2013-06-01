#pragma once

#include "Device.h"
#include "RenderTarget.h"
#include <d3d11.h>

struct ENDLESS_RENDERER_DESC
{
	HWND	hWnd;
	float	ViewportParam[6];
};

class EEndlessRenderer : public EDevice, ERenderTargets
{
public:
	EEndlessRenderer();
	~EEndlessRenderer();

	bool		Init(ENDLESS_RENDERER_DESC& desc);
	void		Render(void);

protected:
	void		SetSceneViewport(void);
	void		CreateBackBuffer(void);

	D3D11_VIEWPORT		mViewport;
};