#include "RenderTarget.h"
#include <assert.h>
#include <d3d11.h>

using namespace Declare;

extern ID3D11Device* g_hDevice;
extern ID3D11DeviceContext*    g_hContext;

ERenderTargets::ERenderTargets()
{

}

ERenderTargets::~ERenderTargets()
{
	for (vector<ERenderBuffer*>::iterator i = mFixedBuffers.begin(); i != mFixedBuffers.end(); i++)
	{
		delete *i;
		*i = nullptr;
	}

	for (vector<ERenderBuffer*>::iterator i = mRuntimeBuffers.begin(); i != mRuntimeBuffers.end(); i++)
	{
		delete *i;
		*i = nullptr;
	}

	// todo: mTextures;
}

ERenderBuffer* ERenderTargets::CreateFixedRenderBuffer(ETexture* texture, int bufferIndex)
{
	HRESULT hr;
	ID3D11RenderTargetView* rtv = NULL;
	hr = g_hDevice->CreateRenderTargetView(static_cast<ID3D11Resource*>(texture->GetDXTexture()), NULL, &rtv );

	if (FAILED(hr))
		return nullptr;

	ERenderBuffer* renderTarget = new ERenderBuffer;
	renderTarget->mhRenderTargetView = rtv;
	renderTarget->mTexture = texture;
	renderTarget->mBufferIndex = bufferIndex;
	renderTarget->mBufferUsage = FIXED_BUFFER;
	mFixedBuffers.push_back(renderTarget);

	return renderTarget;
}


ERenderBuffer* ERenderTargets::CreateRuntimeRenderBuffer(ETexture* texture)
{
	HRESULT hr;
	ID3D11RenderTargetView* rtv = NULL;
	hr = g_hDevice->CreateRenderTargetView(static_cast<ID3D11Resource*>(texture->GetDXTexture()), NULL, &rtv );

	if (FAILED(hr))
		return nullptr;

	ERenderBuffer* renderTarget = new ERenderBuffer;
	renderTarget->mhRenderTargetView = rtv;
	renderTarget->mTexture = texture;
	renderTarget->mBufferIndex = 0;						// invalid
	renderTarget->mBufferUsage = RUNTIME_BUFFER;
	mRuntimeBuffers.push_back(renderTarget);

	return renderTarget;
}

void ERenderTargets::DeleteRenderBuffer(ERenderBuffer* rt)
{
	if (rt && rt->mhRenderTargetView)
	{
		int index = rt->mBufferIndex;
		if (rt->mBufferUsage == FIXED_BUFFER)
		{
			mFixedBuffers[index] = nullptr;
		}
		else if (rt->mBufferUsage == RUNTIME_BUFFER)
		{
			if (mRuntimeBuffers.size() != 1)
			{
				mRuntimeBuffers[index] = mRuntimeBuffers.back();
				mRuntimeBuffers.pop_back();
				mRuntimeBuffers[index]->mBufferIndex = index;
			}
			else
			{
				mRuntimeBuffers.clear();
			}
		}

		delete rt;
	}
}

void ERenderTargets::DeleteRenderBuffer(BYTE usage, int index)
{
	ERenderBuffer* rt = nullptr;
	if (usage == FIXED_BUFFER)
	{
		assert(index >= 0 && index < (int)mFixedBuffers.size());
		rt = mFixedBuffers[index];
		mFixedBuffers[index] = nullptr;
	}
	else if (usage == RUNTIME_BUFFER)
	{
		assert(index >= 0 && index < (int)mRuntimeBuffers.size());
		rt = mRuntimeBuffers[index];
		if (mRuntimeBuffers.size() != 1)
		{
			mRuntimeBuffers[index] = mRuntimeBuffers.back();
			mRuntimeBuffers.pop_back();
			mRuntimeBuffers[index]->mBufferIndex = index;
		}
		else
		{
			mRuntimeBuffers.clear();
		}
	}

	delete rt;
}

void ERenderTargets::SetRenderBuffer(ERenderBuffer* renderTarget)
{
	mActiveBuffers.push_back(renderTarget->mhRenderTargetView);
}

void ERenderTargets::BeginRenderBuffers(ID3D11DepthStencilView* depthStencil)
{
	assert(mActiveBuffers.size());
	g_hContext->OMSetRenderTargets(mActiveBuffers.size(), (ID3D11RenderTargetView* const*)&mActiveBuffers[0], depthStencil);
}

void ERenderTargets::FinishRenderBuffers()
{
	mActiveBuffers.clear();
}

void ERenderTargets::SetRenderBuffer(int index)
{
	assert(index >= 0 && index < (int)mFixedBuffers.size());
	SetRenderBuffer(mFixedBuffers[index]);
}

void ERenderTargets::ClearRenderBuffer(int index, const XMFLOAT4& color)
{
	assert(index >= 0 && index < (int)mFixedBuffers.size());
	ERenderBuffer* buffer = mFixedBuffers[index];
	if (buffer)
	{
		buffer->Clear(color);
	}
}

void ERenderBuffer::Clear(const XMFLOAT4& color)
{
	g_hContext->ClearRenderTargetView( mhRenderTargetView, static_cast<const FLOAT*>(&color.x) );
}

ETexture::~ETexture()
{
}