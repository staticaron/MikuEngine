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
		io.Fonts->AddFontFromFileTTF( RESOURCE_DIR "/fonts/ruda_bold.ttf" );
		io.Fonts->AddFontFromFileTTF( RESOURCE_DIR "/fonts/jetbrains_mono.ttf" );
		io.Fonts->AddFontFromFileTTF( RESOURCE_DIR "/fonts/ruda.ttf" );
		io.Fonts->AddFontFromFileTTF( RESOURCE_DIR "/fonts/ruda_black.ttf" );

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		style.ScaleAllSizes( main_scale );
		style.FontScaleDpi = main_scale;
		style.FontScaleMain = 0.8f;
		style.FrameRounding = 4;
		style.WindowRounding = 4;
		style.WindowPadding = { 4, 4 };
		style.FrameBorderSize = 1.0f;

		ImGui_ImplGlfw_InitForOpenGL( window, true );
		ImGui_ImplOpenGL3_Init();
	}

	void ImguiManager::SetTheme()
	{
		ImGuiStyle style = ImGui::GetStyle();
		ImVec4* colors = ImGui::GetStyle().Colors;

		// Base (neutral, not teal)
		colors[ ImGuiCol_WindowBg ] = ImVec4( 0.11f, 0.12f, 0.13f, 1.00f ); // #1B1D1F
		colors[ ImGuiCol_ChildBg ] = ImVec4( 0.08f, 0.09f, 0.10f, 1.00f );  // #16181A
		colors[ ImGuiCol_PopupBg ] = ImVec4( 0.08f, 0.09f, 0.10f, 1.00f );

		// Text
		colors[ ImGuiCol_Text ] = ImVec4( 1.00f, 0.95f, 0.92f, 1.00f );		// #FFF2EA
		colors[ ImGuiCol_TextDisabled ] = ImVec4( 0.69f, 0.74f, 0.79f, 1.00f ); // #B0BCCA

		// Borders
		colors[ ImGuiCol_Border ] = ImVec4( 0.69f, 0.74f, 0.79f, 0.08f );

		// Primary Accent (Miku teal)
		colors[ ImGuiCol_Button ] = ImVec4( 0.05f, 0.84f, 0.87f, 0.40f ); // #0ED7DF
		colors[ ImGuiCol_ButtonHovered ] = ImVec4( 0.05f, 0.84f, 0.87f, 1.00f );
		colors[ ImGuiCol_ButtonActive ] = ImVec4( 0.02f, 0.70f, 0.75f, 1.00f );

		// Headers / selection
		colors[ ImGuiCol_Header ] = ImVec4( 0.05f, 0.84f, 0.87f, 0.25f );
		colors[ ImGuiCol_HeaderHovered ] = ImVec4( 0.05f, 0.84f, 0.87f, 0.80f );
		colors[ ImGuiCol_HeaderActive ] = ImVec4( 0.05f, 0.84f, 0.87f, 1.00f );

		// Inputs
		colors[ ImGuiCol_FrameBg ] = ImVec4( 0.07f, 0.08f, 0.09f, 1.00f ); // #121416
		colors[ ImGuiCol_FrameBgHovered ] = ImVec4( 0.05f, 0.84f, 0.87f, 0.30f );
		colors[ ImGuiCol_FrameBgActive ] = ImVec4( 0.05f, 0.84f, 0.87f, 0.60f );

		// Tabs
		colors[ ImGuiCol_Tab ] = ImVec4( 0.08f, 0.09f, 0.10f, 1.00f );
		colors[ ImGuiCol_TabHovered ] = ImVec4( 0.05f, 0.84f, 0.87f, 0.80f );
		colors[ ImGuiCol_TabActive ] = ImVec4( 0.05f, 0.64f, 0.67f, 1.00f );

		// Title
		colors[ ImGuiCol_TitleBg ] = ImVec4( 0.07f, 0.08f, 0.09f, 1.00f );
		colors[ ImGuiCol_TitleBgActive ] = ImVec4( 0.11f, 0.12f, 0.13f, 1.00f );

		// Sliders
		colors[ ImGuiCol_SliderGrab ] = ImVec4( 0.05f, 0.84f, 0.87f, 1.00f );
		colors[ ImGuiCol_SliderGrabActive ] = ImVec4( 0.02f, 0.70f, 0.75f, 1.00f );

		// Pink accent (VERY minimal use)
		colors[ ImGuiCol_CheckMark ] = ImVec4( 0.92f, 0.31f, 0.61f, 1.00f ); // #EA4F9C
		colors[ ImGuiCol_SeparatorActive ] = ImVec4( 0.92f, 0.31f, 0.61f, 0.60f );

		style.WindowRounding = 8.0f;
		style.FrameRounding = 6.0f;
		style.GrabRounding = 6.0f;
		style.TabRounding = 6.0f;
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
