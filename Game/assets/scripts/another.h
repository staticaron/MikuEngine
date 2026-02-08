#pragma once

#include "MikuEngine/Helpers/DLLloader.h"
#include "MikuEngine/NativeScript.h"

class AnotherClass : public MikuEngine::NativeScript
{
public:
	void OnUpdate( MikuEngine::Entity entity ) override;

private:
};

REGISTER_SCRIPT( AnotherClass, "AnotherClass" );
