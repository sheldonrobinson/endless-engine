#pragma once
#include <Windows.h>
#include <xnamath.h>
#include "HeartBeat.h"
#include <vector>
#include "Allocator.h"

namespace LionHeart
{
	class Heart
	{
	public:
		Heart(HWND hWnd);
		~Heart(void);

		bool Init(void);
		void Render(void);

		// render

		// render device
		bool InitDevice(void);
		void ClearDevice(void);

		void Present(void);

		// render target
		RenderTarget* CreateRenderTarget(Texture* texture);
		void DeleteRenderTarget(RenderTarget* rt);
		void DeleteRenderTarget(int index);
		void ClearRenderTarget(const XMFLOAT4& color);
		void SetMainRT(void);
		void SetRenderTarget(RenderTarget* renderTarget);

		// view port
		void SetMainViewPort(void);
		void SetViewport(Viewport* viewport);
		Viewport* CreateViewport(float param[6]);
		void DeleteViewport(Viewport* viewport);
		void DeleteViewport(int index);
	public:

	private:
		Heart(void);
		Heart(const Heart&);

	private:
		HWND m_hWnd;
		HeartBeat* m_pMainBeat;

		indexed_vector<Viewport*> m_Viewports;
		indexed_vector<RenderTarget*> m_RenderTargets;

		std::vector<HeartBeat*> m_vBeatLine;
	};
}