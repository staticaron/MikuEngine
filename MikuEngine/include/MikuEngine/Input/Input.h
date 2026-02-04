#pragma once

#include <utility>

#include "Core.h"

namespace MikuEngine
{
	class MIKU_API Input
	{
	public:
		inline static bool IsKeyPressed( int keycode ) { return s_Instance->IsKeyPressedImpl( keycode ); }
		inline static std::pair<int, int> GetAxisRaw() { return s_Instance->GetAxisRawImpl(); }

		inline static bool IsMouseButtonPressed( int button ) { return s_Instance->IsMouseButtonPressedImpl( button ); }
		inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }

	protected:
		virtual bool IsKeyPressedImpl( int keycode ) = 0;

		virtual std::pair<int, int> GetAxisRawImpl() = 0;

		virtual bool IsMouseButtonPressedImpl( int button ) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

	public:
		static Input* s_Instance;
	};
}
