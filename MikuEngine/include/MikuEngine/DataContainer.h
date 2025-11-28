#pragma once

#include "glm/glm.hpp"

#include "Core.h"

namespace MikuEngine
{
	struct MIKU_API DataContainer
	{
	public:
		glm::vec2 GetViewportSize() const { return m_ViewportSize; }
		void SetViewportSize( glm::vec2 viewportSize ) { m_ViewportSize = viewportSize; }

	private:
		glm::vec2 m_ViewportSize = { 800, 600 };
	};
}
