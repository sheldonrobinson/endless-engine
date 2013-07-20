//GameMain.cpp:	DemoÈë¿Ú

#include "RendererPublic.h"

HWND InitWindow(HINSTANCE hInstance, int nCmdShow, TCHAR* appName);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int Run(EEndlessRenderer* renderer);


int APIENTRY WinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// init a windows form
	HWND hWnd = InitWindow(hInstance, nCmdShow, L"EndlessEngineApp");

	// initialize engine
	g_pRenderer->Init(hWnd);

	// main loop
	return Run(g_pRenderer);
}

HWND InitWindow(HINSTANCE hInstance, int nCmdShow, TCHAR* appName)
{
	// Register Class
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= 0;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= appName;
	wcex.hIconSm		= 0;

	RegisterClassEx(&wcex);

	// Create Window
	HWND hWnd;

	hWnd = CreateWindow(appName, L"Endless 3D Engine", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);	

	return hWnd;
}

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

int Run(EEndlessRenderer* renderer)
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
			renderer->Render();
		}
	}

	return (int) msg.wParam;
}
