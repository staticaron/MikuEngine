#pragma once

#include "glm/glm.hpp"

#include "Core.h"

namespace MikuEngine
{
	class MIKU_API CentralInput
	{
	public:
		void Update( double dt );

		bool IsMouseButtonPressed( int keycode ) const;
		const glm::dvec2& GetMousePosition() const { return m_MousePosition; }
		glm::dvec2 GetMousePositionDelta() const { return m_MousePosition - m_LastMousePosition; }

		const glm::ivec3& GetAxisRaw() const { return m_AxisRaw; }
		bool IsKeyPressed( int keycode ) const;

		void LockCursor() const;
		void UnlockCursor() const;

	private:
		void UpdateKeyboardButtons();
		void UpdateAxisRaw();

		void UpdateMousePosition();
		void UpdateMouseButtons();

	private:
		glm::ivec3 m_AxisRaw = { 0, 0, 0 };

		glm::dvec2 m_MousePosition = {};
		glm::dvec2 m_LastMousePosition = {};

		std::unordered_map<int, int> m_KeyCodeState = {};
		std::unordered_map<int, int> m_MouseKeyCodeState = {};
	};
}
