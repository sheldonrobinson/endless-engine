//LightBase.h:	Light Base class in Endless engine

#pragma once

#include "UtilityPublic.h"
#include "EndlessLight.h"

class ELightBase : public EEndlessLight
{
//Public API
public:
	virtual void Init();

public:
	ELightBase();
	virtual ~ELightBase();

public:
	UINT32 m_Id;
};