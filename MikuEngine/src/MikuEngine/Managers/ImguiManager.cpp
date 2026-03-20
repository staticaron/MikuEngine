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

	void ImguiManager::SetTheme()
	{
		// Dark Ruda style by Raikiri from ImThemes
		ImGuiStyle& style = ImGui::GetStyle();

		style.Alpha = 1.0f;
		style.DisabledAlpha = 0.6f;
		style.WindowPadding = ImVec2( 8.0f, 8.0f );
		style.WindowRounding = 0.0f;
		style.WindowBorderSize = 1.0f;
		style.WindowMinSize = ImVec2( 32.0f, 32.0f );
		style.WindowTitleAlign = ImVec2( 0.0f, 0.5f );
		style.WindowMenuButtonPosition = ImGuiDir_Left;
		style.ChildRounding = 0.0f;
		style.ChildBorderSize = 1.0f;
		style.PopupRounding = 0.0f;
		style.PopupBorderSize = 1.0f;
		style.FramePadding = ImVec2( 4.0f, 3.0f );
		style.FrameRounding = 4.0f;
		style.FrameBorderSize = 0.0f;
		style.ItemSpacing = ImVec2( 8.0f, 4.0f );
		style.ItemInnerSpacing = ImVec2( 4.0f, 4.0f );
		style.CellPadding = ImVec2( 4.0f, 2.0f );
		style.IndentSpacing = 21.0f;
		style.ColumnsMinSpacing = 6.0f;
		style.ScrollbarSize = 14.0f;
		style.ScrollbarRounding = 9.0f;
		style.GrabMinSize = 10.0f;
		style.GrabRounding = 4.0f;
		style.TabRounding = 4.0f;
		style.TabBorderSize = 0.0f;
		style.ColorButtonPosition = ImGuiDir_Right;
		style.ButtonTextAlign = ImVec2( 0.5f, 0.5f );
		style.SelectableTextAlign = ImVec2( 0.0f, 0.0f );

		style.Colors[ ImGuiCol_Text ] = ImVec4( 0.9490196f, 0.95686275f, 0.9764706f, 1.0f );
		style.Colors[ ImGuiCol_TextDisabled ] = ImVec4( 0.35686275f, 0.41960785f, 0.46666667f, 1.0f );
		style.Colors[ ImGuiCol_WindowBg ] = ImVec4( 0.10980392f, 0.14901961f, 0.16862746f, 1.0f );
		style.Colors[ ImGuiCol_ChildBg ] = ImVec4( 0.14901961f, 0.1764706f, 0.21960784f, 1.0f );
		style.Colors[ ImGuiCol_PopupBg ] = ImVec4( 0.078431375f, 0.078431375f, 0.078431375f, 0.94f );
		style.Colors[ ImGuiCol_Border ] = ImVec4( 0.078431375f, 0.09803922f, 0.11764706f, 1.0f );
		style.Colors[ ImGuiCol_BorderShadow ] = ImVec4( 0.0f, 0.0f, 0.0f, 0.0f );
		style.Colors[ ImGuiCol_FrameBg ] = ImVec4( 0.2f, 0.24705882f, 0.28627452f, 1.0f );
		style.Colors[ ImGuiCol_FrameBgHovered ] = ImVec4( 0.11764706f, 0.2f, 0.2784314f, 1.0f );
		style.Colors[ ImGuiCol_FrameBgActive ] = ImVec4( 0.08627451f, 0.11764706f, 0.13725491f, 1.0f );
		style.Colors[ ImGuiCol_TitleBg ] = ImVec4( 0.08627451f, 0.11764706f, 0.13725491f, 0.65f );
		style.Colors[ ImGuiCol_TitleBgActive ] = ImVec4( 0.078431375f, 0.09803922f, 0.11764706f, 1.0f );
		style.Colors[ ImGuiCol_TitleBgCollapsed ] = ImVec4( 0.0f, 0.0f, 0.0f, 0.51f );
		style.Colors[ ImGuiCol_MenuBarBg ] = ImVec4( 0.14901961f, 0.1764706f, 0.21960784f, 1.0f );
		style.Colors[ ImGuiCol_ScrollbarBg ] = ImVec4( 0.019607844f, 0.019607844f, 0.019607844f, 0.39f );
		style.Colors[ ImGuiCol_ScrollbarGrab ] = ImVec4( 0.2f, 0.24705882f, 0.28627452f, 1.0f );
		style.Colors[ ImGuiCol_ScrollbarGrabHovered ] = ImVec4( 0.1764706f, 0.21960784f, 0.24705882f, 1.0f );
		style.Colors[ ImGuiCol_ScrollbarGrabActive ] = ImVec4( 0.08627451f, 0.20784314f, 0.30980393f, 1.0f );
		style.Colors[ ImGuiCol_CheckMark ] = ImVec4( 0.2784314f, 0.5568628f, 1.0f, 1.0f );
		style.Colors[ ImGuiCol_SliderGrab ] = ImVec4( 0.2784314f, 0.5568628f, 1.0f, 1.0f );
		style.Colors[ ImGuiCol_SliderGrabActive ] = ImVec4( 0.36862746f, 0.60784316f, 1.0f, 1.0f );
		style.Colors[ ImGuiCol_Button ] = ImVec4( 0.2f, 0.24705882f, 0.28627452f, 1.0f );
		style.Colors[ ImGuiCol_ButtonHovered ] = ImVec4( 0.2784314f, 0.5568628f, 1.0f, 1.0f );
		style.Colors[ ImGuiCol_ButtonActive ] = ImVec4( 0.05882353f, 0.5294118f, 0.9764706f, 1.0f );
		style.Colors[ ImGuiCol_Header ] = ImVec4( 0.2f, 0.24705882f, 0.28627452f, 0.55f );
		style.Colors[ ImGuiCol_HeaderHovered ] = ImVec4( 0.25882354f, 0.5882353f, 0.9764706f, 0.8f );
		style.Colors[ ImGuiCol_HeaderActive ] = ImVec4( 0.25882354f, 0.5882353f, 0.9764706f, 1.0f );
		style.Colors[ ImGuiCol_Separator ] = ImVec4( 0.2f, 0.24705882f, 0.28627452f, 1.0f );
		style.Colors[ ImGuiCol_SeparatorHovered ] = ImVec4( 0.09803922f, 0.4f, 0.7490196f, 0.78f );
		style.Colors[ ImGuiCol_SeparatorActive ] = ImVec4( 0.09803922f, 0.4f, 0.7490196f, 1.0f );
		style.Colors[ ImGuiCol_ResizeGrip ] = ImVec4( 0.25882354f, 0.5882353f, 0.9764706f, 0.25f );
		style.Colors[ ImGuiCol_ResizeGripHovered ] = ImVec4( 0.25882354f, 0.5882353f, 0.9764706f, 0.67f );
		style.Colors[ ImGuiCol_ResizeGripActive ] = ImVec4( 0.25882354f, 0.5882353f, 0.9764706f, 0.95f );
		style.Colors[ ImGuiCol_Tab ] = ImVec4( 0.10980392f, 0.14901961f, 0.16862746f, 1.0f );
		style.Colors[ ImGuiCol_TabHovered ] = ImVec4( 0.25882354f, 0.5882353f, 0.9764706f, 0.8f );
		style.Colors[ ImGuiCol_TabActive ] = ImVec4( 0.2f, 0.24705882f, 0.28627452f, 1.0f );
		style.Colors[ ImGuiCol_TabUnfocused ] = ImVec4( 0.10980392f, 0.14901961f, 0.16862746f, 1.0f );
		style.Colors[ ImGuiCol_TabUnfocusedActive ] = ImVec4( 0.10980392f, 0.14901961f, 0.16862746f, 1.0f );
		style.Colors[ ImGuiCol_PlotLines ] = ImVec4( 0.60784316f, 0.60784316f, 0.60784316f, 1.0f );
		style.Colors[ ImGuiCol_PlotLinesHovered ] = ImVec4( 1.0f, 0.42745098f, 0.34901962f, 1.0f );
		style.Colors[ ImGuiCol_PlotHistogram ] = ImVec4( 0.8980392f, 0.69803923f, 0.0f, 1.0f );
		style.Colors[ ImGuiCol_PlotHistogramHovered ] = ImVec4( 1.0f, 0.6f, 0.0f, 1.0f );
		style.Colors[ ImGuiCol_TableHeaderBg ] = ImVec4( 0.1882353f, 0.1882353f, 0.2f, 1.0f );
		style.Colors[ ImGuiCol_TableBorderStrong ] = ImVec4( 0.30980393f, 0.30980393f, 0.34901962f, 1.0f );
		style.Colors[ ImGuiCol_TableBorderLight ] = ImVec4( 0.22745098f, 0.22745098f, 0.24705882f, 1.0f );
		style.Colors[ ImGuiCol_TableRowBg ] = ImVec4( 0.0f, 0.0f, 0.0f, 0.0f );
		style.Colors[ ImGuiCol_TableRowBgAlt ] = ImVec4( 1.0f, 1.0f, 1.0f, 0.06f );
		style.Colors[ ImGuiCol_TextSelectedBg ] = ImVec4( 0.25882354f, 0.5882353f, 0.9764706f, 0.35f );
		style.Colors[ ImGuiCol_DragDropTarget ] = ImVec4( 1.0f, 1.0f, 0.0f, 0.9f );
		style.Colors[ ImGuiCol_NavHighlight ] = ImVec4( 0.25882354f, 0.5882353f, 0.9764706f, 1.0f );
		style.Colors[ ImGuiCol_NavWindowingHighlight ] = ImVec4( 1.0f, 1.0f, 1.0f, 0.7f );
		style.Colors[ ImGuiCol_NavWindowingDimBg ] = ImVec4( 0.8f, 0.8f, 0.8f, 0.2f );
		style.Colors[ ImGuiCol_ModalWindowDimBg ] = ImVec4( 0.8f, 0.8f, 0.8f, 0.35f );
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
