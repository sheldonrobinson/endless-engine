#include "Renderer.h"

extern ID3D11DeviceContext*    g_hContext;
extern IDXGISwapChain*         g_hSwapChain;

EEndlessRenderer::EEndlessRenderer()
{
}

EEndlessRenderer::~EEndlessRenderer()
{
}

bool EEndlessRenderer::Init(ENDLESS_RENDERER_DESC& desc)
{
	// d3d device
	if (!InitDevice(desc.hWnd))
		return false;
	// scene viewport
	memcpy(&mViewport, desc.ViewportParam, sizeof(D3D11_VIEWPORT)); 

	CreateBackBuffer();

	return true;
}

void EEndlessRenderer::Render()
{
	SetSceneViewport();

	// render
	SetRenderBuffer(Declare::BACKBUFFER);
	BeginRenderBuffers();

	ClearRenderBuffer(Declare::BACKBUFFER, XMFLOAT4(0.0f, 0.125f, 0.3f, 1.0f));

	FinishRenderBuffers();
	Present();
}

void EEndlessRenderer::SetSceneViewport()
{
	g_hContext->RSSetViewports( 1, &mViewport);
}

void EEndlessRenderer::CreateBackBuffer()
{
	ID3D11Texture2D* hTexture = nullptr;
	HRESULT hr = S_OK;
	hr = g_hSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&hTexture );
	if( FAILED( hr ) )
		assert(0);
	ETexture2D* backBufferTexure = new ETexture2D;
	backBufferTexure->SetDXTexture(hTexture);
	CreateFixedRenderBuffer(backBufferTexure, Declare::BACKBUFFER);
	backBufferTexure->Release();
}