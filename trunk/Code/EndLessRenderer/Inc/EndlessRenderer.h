//EndlessRenderer.h:	Renderer Interface

#pragma once

#include "UtilityPublic.h"

class EEndlessRenderObj;
class EEndlessLight;
class EEndlessCamera;

class EEndlessRenderer
{
//Renderer API :
public:
	virtual void Init(HWND) = 0 {}

	virtual void Update() = 0 {}
	virtual void Render() = 0 {}

	virtual UINT32 CreateRenderObj() = 0 {}
	virtual bool   DeleteRenderObj(UINT32) = 0 {}
	virtual EEndlessRenderObj* GetRenderObj(UINT32) = 0 {}

	virtual UINT32 CreateLight() = 0 {}
	virtual bool   DeleteLight(UINT32) = 0 {}
	virtual EEndlessLight* GetLight(UINT32) = 0 {}

	//Only support single camera
	virtual EEndlessCamera* GetCamera() = 0 {};

public:
	EEndlessRenderer(){}
	virtual ~EEndlessRenderer(){}
};

extern EEndlessRenderer* g_pRenderer;