#pragma once

#include "backends/imgui_impl_glfw.h"
#include "Core.h"
#include "Rendering/FrameBuffer.h"

namespace MikuEngine
{
	class MIKU_API ImguiManager
	{
	public:
		void Init( GLFWwindow* window );

		void PrepareFrame();
		void RenderFrame();

		void RenderFrameBuffer( FrameBuffer& frameBuffer );

	private:
	};
}
