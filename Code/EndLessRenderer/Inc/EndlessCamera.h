//EndlessCamera.h:	Define Camera Interface

#pragma once

class EEndlessCamera
{
//Public API
public:
	virtual void Init() = 0 {}

public:
	EEndlessCamera(){}
	virtual ~EEndlessCamera(){}
};