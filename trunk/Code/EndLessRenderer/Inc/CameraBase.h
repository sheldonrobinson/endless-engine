//CameraBase.h:	Define Base Camera In Endless Engine

#pragma once

#include "EndlessCamera.h"

class ECameraBase : public EEndlessCamera
{
//Public API
public:
	virtual void Init();

public:
	ECameraBase();
	virtual ~ECameraBase();
};