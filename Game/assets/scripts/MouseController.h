#pragma once

#include "MikuEngine.h"

class MouseController : public MikuEngine::NativeScript
{
public:
	void OnUpdate( double dt ) override;

private:
};

REGISTER_SCRIPT( MouseController, "MouseController" );
