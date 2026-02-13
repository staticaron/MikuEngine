#pragma once

#include "MikuEngine.h"

class CardController : public MikuEngine::NativeScript
{
public:
	void OnUpdate( double dt ) override;

private:
	bool m_WasPressedInPreviousFrame = false;
	glm::vec2 m_StartPosition;

	float m_RotateSpeed = 100.0f;
	float m_NormalizingSpeed = 5.0f;

	glm::vec2 m_MinMaxRotX = { -.5f, .5f };
	glm::vec2 m_MinMaxRotY = { -.5f, .5f };
};

REGISTER_SCRIPT( CardController, "CardController" );
