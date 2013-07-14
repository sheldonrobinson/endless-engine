//Renderer.h : Endless Renderer API Header
#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

//Renderer Init Parameter
struct ENDLESS_RENDERER_DESC
{
	HWND	hWndMain;			//	main window
};

class EEndlessRenderer
{
public:
	EEndlessRenderer();
	~EEndlessRenderer();

	bool	Init(ENDLESS_RENDERER_DESC& desc);
	void	Render();
};