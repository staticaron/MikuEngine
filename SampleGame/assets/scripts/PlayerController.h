#pragma once

#include "MikuEngine/Helpers/DLLloader.h"
#include "MikuEngine/NativeScript.h"

class PlayerController : public MikuEngine::NativeScript
{
public:
	void OnUpdate( double dt ) override;

private:
	float m_MoveSpeed = 0.05;
};

REGISTER_SCRIPT( PlayerController, "PlayerController" );
