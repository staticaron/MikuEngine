#pragma once

#include "backends/imgui_impl_glfw.h"
#include "Core.h"

namespace MikuEngine
{
	class MIKU_API ImguiManager
	{
	public:
		void Init( GLFWwindow* window );

		void PrepareFrame();
		void RenderFrame();

	private:
	};
}
