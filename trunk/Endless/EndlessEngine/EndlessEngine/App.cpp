// LionHeartApp.cpp : Defines the entry point for the application.
//

#pragma once


#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include "App.h"
#include "Renderer.h"
#include "resource.h"

HWND InitWindow(HINSTANCE hInstance, int nCmdShow, TCHAR* appName);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int Run(EEndlessRenderer& renderer);


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// init a windows form
	HWND hWnd = InitWindow(hInstance, nCmdShow, L"EndlessEngineApp");

	// initialize engine
	ENDLESS_RENDERER_DESC desc;
	desc.hWnd = hWnd;
	float param[6] = { 1280.f, 720.f, 0.0f, 1.0f, 0, 0 };
	memcpy(desc.ViewportParam, param, sizeof(float) * 6);
	EEndlessRenderer renderer;

	if (!renderer.Init(desc))
		return 1;

	// main loop
	return Run(renderer);
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ENDLESSAPP));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= appName;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	if (!RegisterClassEx(&wcex))
	{
		printf("Failed to register class!\n");
		exit(0);
	}

	// Create Window
	HWND hWnd;

	hWnd = CreateWindow(appName, L"Endless 3D Engine", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		printf("Failed to register class!\n");
		exit(0);
	}

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

int Run(EEndlessRenderer& renderer)
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
			renderer.Render();
		}
	}

	return (int) msg.wParam;
}