#pragma once

#include <string>

#include "backends/imgui_impl_glfw.h"
#include "Core.h"
#include "Rendering/SceneFBO.h"

namespace MikuEngine
{
#define DISABLED_IMGUI( x )                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            \
	ImGui::BeginDisabled();                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        \
	x;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             \
	ImGui::EndDisabled();
}

namespace MikuEngine
{
	class MIKU_API ImguiManager
	{
	public:
		void Init( GLFWwindow* window );

		void PrepareFrame();
		void RenderFrame();

		void RenderFrameBuffer( SceneFBO& frameBuffer );

		static bool FullWidthButton( const std::string& text );

	private:
	};
}
