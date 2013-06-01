#include "Heart.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <assert.h>

namespace LionHeart
{
	D3D_DRIVER_TYPE g_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Device*           g_pd3dDevice = NULL;
	ID3D11DeviceContext*    g_pImmediateContext = NULL;
	IDXGISwapChain*         g_pSwapChain = NULL;
	//ID3D11RenderTargetView* g_pMainRTView = NULL;
	//RenderTarget*			g_pMainRT = NULL;
	//D3D11_VIEWPORT			g_viewPort;
	//Viewport*				g_pMainViewport = NULL;

	bool Heart::InitDevice()
	{
		HRESULT hr = S_OK;

		RECT rc;
		GetClientRect( m_hWnd, &rc );
		UINT width = rc.right - rc.left;
		UINT height = rc.bottom - rc.top;

		UINT createDeviceFlags = 0;
	#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};
		UINT numDriverTypes = ARRAYSIZE( driverTypes );

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};
		UINT numFeatureLevels = ARRAYSIZE( featureLevels );

		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory( &sd, sizeof( sd ) );
		sd.BufferCount = 1;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = m_hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
		{
			g_driverType = driverTypes[driverTypeIndex];
			hr = D3D11CreateDeviceAndSwapChain( NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
												D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext );
			if( SUCCEEDED( hr ) )
				break;
		}
		if( FAILED( hr ) )
			return false;

		// Create a render target view
		Texture backBuffer(TEXTURE_2D);
		hr = g_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&backBuffer.texture );
		if( FAILED( hr ) )
			return false;

		//g_pMainRT = CreateRenderTarget(&backBuffer);
		m_pMainBeat->m_pRenderTarget = CreateRenderTarget(&backBuffer);
		ref_to_tex2d(backBuffer)->Release();
		if( m_pMainBeat->m_pRenderTarget == NULL )
			return false;

		// Setup the viewport
		float param[6] = { (FLOAT)width, (FLOAT)height, 0.0f, 1.0f, 0, 0 };
		//g_pMainViewport = CreateViewport(param);
		m_pMainBeat->m_pViewport = CreateViewport(param);

		return true;
	}

	void Heart::ClearDevice()
	{
		//if( g_pMainRT ) ptr_to_rtv(g_pMainRT)->Release();
		if( g_pSwapChain ) g_pSwapChain->Release();
		if( g_pImmediateContext ) g_pImmediateContext->Release();
		if( g_pd3dDevice ) g_pd3dDevice->Release();
	}

	void Heart::Present()
	{
		g_pSwapChain->Present(0, 0);
	}

	RenderTarget* Heart::CreateRenderTarget(Texture* texture)
	{
		HRESULT hr;
		ID3D11RenderTargetView* rtv = NULL;
		switch(texture->type)
		{
		case TEXTURE_1D:
			hr = g_pd3dDevice->CreateRenderTargetView( static_cast<ID3D11Texture1D*>(texture->texture), NULL, &rtv );
			break;
		case TEXTURE_2D:
			hr = g_pd3dDevice->CreateRenderTargetView( static_cast<ID3D11Texture2D*>(texture->texture), NULL, &rtv );
			break;
		case TEXTURE_3D:
			hr = g_pd3dDevice->CreateRenderTargetView( static_cast<ID3D11Texture3D*>(texture->texture), NULL, &rtv );
			break;
		default:
			return NULL;
		}

		if (FAILED(hr))
			return NULL;

		RenderTarget* renderTarget = new RenderTarget;
		renderTarget->renderTargetView = rtv;
		m_RenderTargets.push_back(renderTarget);

		return renderTarget;
	}

	void Heart::DeleteRenderTarget(RenderTarget* rt)
	{
		assert(rt);
		ptr_to_rtv(rt)->Release();
		m_RenderTargets.erase(rt->index);
	}

	void Heart::DeleteRenderTarget(int index)
	{
		RenderTarget* rt = m_RenderTargets[index];
		ptr_to_rtv(rt)->Release();
		m_RenderTargets.erase(index);
	}

	void Heart::ClearRenderTarget(const XMFLOAT4& color)
	{
		g_pImmediateContext->ClearRenderTargetView( ptr_to_rtv(m_pMainBeat->m_pRenderTarget), static_cast<const FLOAT*>(&color.x) );
	}

	void Heart::SetMainRT()
	{
		ID3D11RenderTargetView* const rtv = ptr_to_rtv(m_pMainBeat->m_pRenderTarget);
		g_pImmediateContext->OMSetRenderTargets( 1, &rtv, NULL );
	}

	void Heart::SetRenderTarget(RenderTarget* renderTarget)
	{
		ID3D11RenderTargetView* const rtv = ptr_to_rtv(renderTarget);
		g_pImmediateContext->OMSetRenderTargets( 1, &rtv, NULL );
	}

	void Heart::SetMainViewPort()
	{
		g_pImmediateContext->RSSetViewports( 1, ptr_to_viewport(m_pMainBeat->m_pViewport));
	}

	void Heart::SetViewport(Viewport* viewport)
	{
		g_pImmediateContext->RSSetViewports( 1, ptr_to_viewport(viewport));
	}

	Viewport* Heart::CreateViewport(float param[6])
	{
		Viewport* viewport = new Viewport;
		viewport->viewport = (void*)new D3D11_VIEWPORT;
		memcpy(ptr_to_viewport(viewport), param, sizeof(D3D11_VIEWPORT));
		m_Viewports.push_back(viewport);
		return viewport;
	}

	void Heart::DeleteViewport(Viewport* viewport)
	{
		assert(viewport);
		delete ptr_to_viewport(viewport);
		m_Viewports.erase(viewport->index);
	}

	void Heart::DeleteViewport(int index)
	{
		Viewport* vp = m_Viewports[index];
		delete ptr_to_viewport(vp);
		m_Viewports.erase(vp->index);
	}
}