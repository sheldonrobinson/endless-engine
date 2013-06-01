#include "Heart.h"
#include <algorithm>

namespace LionHeart
{
	Heart::Heart(HWND hWnd)
	{
		m_hWnd = hWnd;
		m_pMainBeat = new MainBeat(this);
		m_vBeatLine.push_back(m_pMainBeat);
	}

	Heart::~Heart()
	{
		if (m_pMainBeat)
		{
			delete m_pMainBeat;
			m_pMainBeat = NULL;
		}
		ClearDevice();
	}

	bool Heart::Init()
	{
		if (!InitDevice())
			return false;

		return true;
	}

	void Heart::Render()
	{
		// Just clear the backbuffer
		std::for_each(m_vBeatLine.begin(), m_vBeatLine.end(), std::mem_fun(&HeartBeat::Render));
		Present();
	}
}