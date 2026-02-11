#pragma once

#include "MikuEngine.h"

class CardController : public MikuEngine::NativeScript
{
public:
	void OnReady() override;
	void OnUpdate( double dt ) override;

private:
};

REGISTER_SCRIPT( CardController, "CardController" );
