#pragma once

#define ref_to_tex1d(x) static_cast<ID3D11Texture1D*>(x.texture)
#define ptr_to_tex1d(x) static_cast<ID3D11Texture1D*>(x->texture)

#define ref_to_tex2d(x) static_cast<ID3D11Texture2D*>(x.texture)
#define ptr_to_tex2d(x) static_cast<ID3D11Texture2D*>(x->texture)

#define ref_to_tex3d(x) static_cast<ID3D11Texture3D*>(x.texture)
#define ptr_to_tex3d(x) static_cast<ID3D11Texture3D*>(x->texture)

#define ref_to_rtv(x) static_cast<ID3D11RenderTargetView*>(x.renderTargetView)
#define ptr_to_rtv(x) static_cast<ID3D11RenderTargetView*>(x->renderTargetView)

#define ref_to_viewport(x) static_cast<D3D11_VIEWPORT*>(x.viewport)
#define ptr_to_viewport(x) static_cast<D3D11_VIEWPORT*>(x->viewport)

namespace LionHeart
{
	class Heart;
	struct Viewport
	{
		void* viewport;
		int index;
	};

	struct RenderTarget
	{
		void* renderTargetView;
		int index;
	};

	enum E_TEXTURE_TYPE
	{
		TEXTURE_1D,
		TEXTURE_2D,
		TEXTURE_3D
	};

	struct Texture
	{
		Texture(E_TEXTURE_TYPE eType) { type = eType; }
		void* texture;
		int index;
		int type;
	};

	class HeartBeat
	{
		friend class Heart;
	public:
		HeartBeat(Heart* heart) { m_pViewport = nullptr; m_pRenderTarget = nullptr; m_pHeart = nullptr; m_pHeart = heart; }
		virtual ~HeartBeat(void) { m_pViewport = nullptr; m_pRenderTarget = nullptr; m_pHeart = nullptr; m_pHeart = nullptr; }

		virtual void Build(void) = 0;
		virtual void Render(void) = 0;

	protected:
		Viewport* m_pViewport;
		RenderTarget* m_pRenderTarget;
		Heart* m_pHeart;
	};

	class MainBeat : public HeartBeat
	{
	public:
		MainBeat(Heart* heart);
		virtual ~MainBeat(void);

		virtual void Build(void);
		virtual void Render(void);
	};


}