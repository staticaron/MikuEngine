#pragma once

#include "MikuEngine/Helpers/DLLloader.h"
#include "MikuEngine/NativeScript.h"

class PlayerController : public MikuEngine::NativeScript
{
public:
	void OnUpdate( double dt ) override;

private:
};

REGISTER_SCRIPT( PlayerController, "PlayerController" );
