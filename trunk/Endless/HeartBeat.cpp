#include "HeartBeat.h"
#include "Heart.h"
#include <assert.h>

namespace LionHeart
{
	MainBeat::MainBeat(Heart* heart) : HeartBeat(heart)
	{
	}

	MainBeat::~MainBeat()
	{
		m_pHeart->DeleteViewport(m_pViewport);
		m_pHeart->DeleteRenderTarget(m_pRenderTarget);
	}

	void MainBeat::Build()
	{
	}

	void MainBeat::Render()
	{
		assert(m_pViewport);
		assert(m_pRenderTarget);
		m_pHeart->SetViewport(m_pViewport);
		m_pHeart->SetRenderTarget(m_pRenderTarget);
		m_pHeart->ClearRenderTarget(XMFLOAT4(0.0f, 0.125f, 0.3f, 1.0f));
	}
}