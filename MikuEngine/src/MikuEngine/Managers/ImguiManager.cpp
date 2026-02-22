#include "Managers/ImguiManager.h"

#include <cstdint>

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"

#include "Application.h"
#include "Rendering/FrameBuffers/SceneFBO.h"

#define DISABLE_IMGUI( x )                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             \
	ImGui::BeginDisabled();                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        \
	x;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             \
	ImGui::EndDisabled();

namespace MikuEngine
{
	void ImguiManager::Init( GLFWwindow* window )
	{
		float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor( glfwGetPrimaryMonitor() );

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		( void )io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.Fonts->AddFontFromFileTTF( RESOURCE_DIR "/fonts/jetbrains_mono.ttf" );

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		style.ScaleAllSizes( main_scale );
		style.FontScaleDpi = main_scale;
		style.FontScaleMain = 0.8f;
		style.FrameRounding = 4;
		style.WindowPadding = { 4, 4 };

		ImGui_ImplGlfw_InitForOpenGL( window, true );
		ImGui_ImplOpenGL3_Init();
	}

	void ImguiManager::PrepareFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGuiDockNodeFlags mainDockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;
		ImGui::DockSpaceOverViewport( ImGui::GetID( "Dockspace" ), ImGui::GetMainViewport(), mainDockspaceFlags );
	}

	void ImguiManager::RenderFrame()
	{
		ImGui::Render();

		ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );

		if ( ImGuiConfigFlags_ViewportsEnable )
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void ImguiManager::RenderFrameBuffer( SceneFBO& frameBuffer )
	{
		ImGui::Begin( "IMPOSTER" );

		ImVec2 windowSize = ImGui::GetContentRegionAvail();

		glm::vec2 viewPortSize = Application::GetDataContainer().GetViewportSize();

		if ( windowSize.x != viewPortSize.x || windowSize.y != viewPortSize.y )
		{
			frameBuffer.ResizeBufferTexture( { windowSize.x, windowSize.y } );
		}

		ImGui::Image( ( void* )( intptr_t )frameBuffer.GetTextureID(), { windowSize.x, windowSize.y }, { 0, 1 }, { 1, 0 }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } );
		ImGui::End();
	}

	bool ImguiManager::FullWidthButton( const std::string& text )
	{
		return ImGui::Button( text.c_str(), ImVec2( -FLT_MIN, 0 ) );
	}
}
