#define PRINT_CMD

#include "LionHeart.h"
#include "Heart.h"
#include "resource.h"

namespace LionHeart
{
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT ps;
		HDC hdc;

		switch (message)
		{
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}

	LionHeart::LionHeart(HINSTANCE hInstance, int nCmdShow, TCHAR* appName)
	{
		// Register Class
		WNDCLASSEX wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style			= CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc	= WndProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 0;
		wcex.hInstance		= hInstance;
		wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LIONHEARTAPP));
		wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
		wcex.lpszMenuName	= NULL;
		wcex.lpszClassName	= appName;
		wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

		if (!RegisterClassEx(&wcex))
		{
			uprintf("Failed to register class!\n");
			exit(0);
		}

		// Create Window
		HWND hWnd;

		hWnd = CreateWindow(appName, L"LionHeart 3D Engine", WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

		if (!hWnd)
		{
			uprintf("Failed to register class!\n");
			exit(0);
		}

		ShowWindow(hWnd, nCmdShow);
		UpdateWindow(hWnd);

		// init device
		m_pHeart = new Heart(hWnd);
		if (!m_pHeart->Init())
		{
			uprintf("Faided to init Render Device!\n");
			exit(0);
		}
	}

	LionHeart::~LionHeart()
	{
		if (m_pHeart)
		{
			delete m_pHeart;
			m_pHeart = NULL;
		}
	}

	int LionHeart::Run()
	{	
		// Main message loop:
		MSG msg = {0};
		while( WM_QUIT != msg.message )
		{
			if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
			else
			{
				m_pHeart->Render();
			}
		}

		return (int) msg.wParam;
	}
}
