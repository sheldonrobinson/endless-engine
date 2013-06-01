#include "Device.h"
#include <d3d11.h>
#include <algorithm>

D3D_DRIVER_TYPE			g_hDriverType		= D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL       g_hFeatureLevel		= D3D_FEATURE_LEVEL_11_0;
ID3D11Device*           g_hDevice		= nullptr;
ID3D11DeviceContext*    g_hContext		= nullptr;
IDXGISwapChain*         g_hSwapChain		= nullptr;

EDevice::EDevice()
{

}

EDevice::~EDevice()
{
	ClearDevice();
}

bool EDevice::InitDevice(HWND hWnd)
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
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
	{
		g_hDriverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain( NULL, g_hDriverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
											D3D11_SDK_VERSION, &sd, &g_hSwapChain, &g_hDevice, &g_hFeatureLevel, &g_hContext );
		if( SUCCEEDED( hr ) )
			break;
	}
	if( FAILED( hr ) )
		return false;

	return true;
}

void EDevice::ClearDevice()
{
	//if( g_pMainRT ) ptr_to_rtv(g_pMainRT)->Release();
	if( g_hSwapChain ) g_hSwapChain->Release();
	if( g_hContext ) g_hContext->Release();
	if( g_hDevice ) g_hDevice->Release();
}

void EDevice::Present()
{
	g_hSwapChain->Present(0, 0);
}
