#include "Common.h"

namespace LionHeart
{
	class Heart;

	class LionHeart
	{
	public:
		LionHeart(HINSTANCE hInstance, int nCmdShow, TCHAR* appName);
		~LionHeart(void);

		int Run(void);
		
	public:

	private:
		LionHeart(void);
		LionHeart(const LionHeart&);

		Heart*	m_pHeart;

	};
}