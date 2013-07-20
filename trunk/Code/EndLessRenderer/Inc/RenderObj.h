//RenderObj.h:	Endless render object define

#pragma once

#include "UtilityPublic.h"
#include "EndlessRenderObj.h"

//Base Class for all render object in Endless engine
class ERenderObj : public EEndlessRenderObj
{
//Render Object, Public API
public:
	virtual void Init();

public:
	ERenderObj();
	virtual ~ERenderObj();

public:
	UINT32 m_Id;
};