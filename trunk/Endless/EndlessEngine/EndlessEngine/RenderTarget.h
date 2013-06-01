#pragma once

#include <vector>
#include <DirectXMath.h>
#include <d3d11.h>

using namespace std;
using namespace DirectX;

namespace Declare
{
	enum FIXED_BUFFER_ENUM
	{
		BACKBUFFER,
		HDR_SCENEBUFFER,
		FIXED_BUFFERNUM
	};

	enum BUFFER_USAGE_ENUM
	{
		FIXED_BUFFER,
		RUNTIME_BUFFER
	};

	enum TEXTURE_TYPE_ENUM
	{
		TEXTURE_1D,
		TEXTURE_2D,
		TEXTURE_3D
	};
};

class ERenderBuffer;
class ETexture
{
public:
	virtual ~ETexture() = 0;
	virtual void		Release() {}
	virtual void*		GetDXTexture(void) { return nullptr; }
	virtual void		SetDXTexture(void*) {}
	virtual int			SizeX(void) { return 0; }
	virtual int			SizeY(void)	{ return 0; }
	virtual int			SizeZ(void)	{ return 0; }

	ERenderBuffer*		mRenderBuffer;
	BYTE				mType;
};

class ETexture1D : public ETexture
{
public:
	virtual ~ETexture1D() { Release(); }
	virtual void	Release(void) { if (mhTexture){mhTexture->Release();mhTexture = nullptr;} }
	virtual void*	GetDXTexture(void) { return mhTexture; }
	virtual void	SetDXTexture(void* texture) { mhTexture = static_cast<ID3D11Texture1D*>(texture); }
	//virtual int		SizeX(void) {}
	virtual int		SizeY(void) { return 0; }
	virtual int		SizeZ(void)	{ return 0; }

protected:
	ID3D11Texture1D*	mhTexture;
};

class ETexture2D : public ETexture
{
public:
	virtual ~ETexture2D() { Release(); }
	virtual void	Release(void) { if (mhTexture){mhTexture->Release();mhTexture = nullptr;} }
	virtual void*	GetDXTexture(void) { return mhTexture; }
	virtual void	SetDXTexture(void* texture) { mhTexture = static_cast<ID3D11Texture2D*>(texture); }
	//virtual int		SizeX(void) {}
	//virtual int		SizeY(void) {}
	virtual int		SizeZ(void)	{ return 0; }

protected:
	ID3D11Texture2D*	mhTexture;
};

class ETexture3D : public ETexture
{
public:
	virtual ~ETexture3D() { Release(); }
	virtual void	Release(void) { if (mhTexture){mhTexture->Release();mhTexture = nullptr;} }
	virtual void*	GetDXTexture(void) { return mhTexture; }
	virtual void	SetDXTexture(void* texture) { mhTexture = static_cast<ID3D11Texture3D*>(texture); }
	//virtual int		SizeX(void) {}
	//virtual int		SizeY(void) {}
	//virtual int		SizeZ(void)	{}

protected:
	ID3D11Texture3D*	mhTexture;
};

class ERenderBuffer
{
	friend class ERenderTargets;
public:
	~ERenderBuffer(void) { if (mhRenderTargetView)mhRenderTargetView->Release(); }

	void	Clear(const XMFLOAT4& color);
	int		SizeX(void) { return mTexture->SizeX(); }
	int		SizeY(void) { return mTexture->SizeY(); }

	ETexture*	mTexture;
	int			mBufferIndex;			// FixedBufferEnum
	BYTE		mBufferUsage;			// BufferUsageEnum

protected:
	ERenderBuffer(void) {}
	ERenderBuffer(const ERenderBuffer&) {}

	ID3D11RenderTargetView*		mhRenderTargetView;

};

class ERenderTargets
{
public:
	ERenderTargets();
	virtual ~ERenderTargets();

	ERenderBuffer*	CreateFixedRenderBuffer(ETexture* texture, int bufferIndex);
	ERenderBuffer*	CreateRuntimeRenderBuffer(ETexture* texture);
	void			DeleteRenderBuffer(ERenderBuffer* rt);
	void			DeleteRenderBuffer(BYTE usage, int index);

	// set render target by order
	void			SetRenderBuffer(ERenderBuffer* renderTarget);
	void			BeginRenderBuffers(ID3D11DepthStencilView* depthStencil = NULL);
	void			FinishRenderBuffers(void);

	// only for fixed
	void			SetRenderBuffer(int index);
	void			ClearRenderBuffer(int index, const XMFLOAT4& color);

protected:
	vector<ERenderBuffer*>		mFixedBuffers;
	vector<ERenderBuffer*>		mRuntimeBuffers;
	vector<ETexture*>			mTextures;

	vector<ID3D11RenderTargetView*>		mActiveBuffers;
};