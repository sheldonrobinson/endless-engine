#pragma once
#include <Windows.h>
#include "Common.h"
#include <vector>

class EDevice
{
public:
	EDevice();
	virtual ~EDevice();

	// render device
	bool InitDevice(HWND hWnd);
	void ClearDevice(void);

	void Present(void);
public:

private:
	EDevice(const EDevice&);

protected:

};
