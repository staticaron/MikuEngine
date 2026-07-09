#pragma once

#include "MikuEngine/Helpers/DLLloader.h"
#include "NativeScript.h"

class Rotator : public MikuEngine::NativeScript
{
public:
	void OnUpdate( double dt ) override;

private:
	float m_RotateSpeed = 1.0f;
};

REGISTER_SCRIPT( Rotator, "Rotator" );
