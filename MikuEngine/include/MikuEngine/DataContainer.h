#pragma once

#include "Core.h"
#include "glm/ext/vector_float2.hpp"

namespace MikuEngine
{
	struct MIKU_API DataContainer
	{
	public:
		glm::vec2 GetViewportSize() const { return m_ViewportSize; }
		float GetViewportAspectRatio() const { return m_ViewportAspectRatio; }

		void SetViewportSize( glm::vec2 viewportSize )
		{
			m_ViewportSize = viewportSize;
			m_ViewportAspectRatio = viewportSize.x / viewportSize.y;
		}

		glm::vec2 GetGameResolution() const { return m_GameResolution; }
		const float& GetGameAspectRatio() const { return m_GameAspectRatio; }

		void SetGameResolutionSize( glm::vec2 resolution )
		{
			m_GameResolution = resolution;
			m_GameAspectRatio = resolution.x / resolution.y;
		}

	private:
		glm::vec2 m_ViewportSize = { 800, 600 };
		glm::vec2 m_GameResolution = { 1600, 900 };

		float m_ViewportAspectRatio = 800.0f / 600.0f;
		float m_GameAspectRatio = 1600.0f / 900;
	};
}
