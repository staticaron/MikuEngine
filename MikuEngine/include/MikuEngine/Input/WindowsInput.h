#pragma once

#include "Core.h"
#include "Input.h"

namespace MikuEngine
{
	class MIKU_API WindowsInput : public Input
	{
	public:
		virtual bool IsKeyPressedImpl( int keycode ) override;
		virtual std::pair<int, int> GetAxisRawImpl() override;

		virtual bool IsMouseButtonPressedImpl( int keycode ) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};
}
