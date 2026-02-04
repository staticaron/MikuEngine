#include "Input/WindowsInput.h"

#include "Application.h"
#include "Input/Input.h"

namespace MikuEngine
{
	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl( int keycode )
	{
		auto window = Application::GetApplication()->GetWindow();
		auto state = glfwGetKey( window, keycode );
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl( int button )
	{
		auto window = Application::GetApplication()->GetWindow();
		auto state = glfwGetMouseButton( window, button );
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		auto window = Application::GetApplication()->GetWindow();
		double xPos, yPos;

		glfwGetCursorPos( window, &xPos, &yPos );
		return { xPos, yPos };
	}

	float WindowsInput::GetMouseXImpl()
	{
		auto [ x, y ] = GetMousePositionImpl();
		return ( float )x;
	}

	float WindowsInput::GetMouseYImpl()
	{
		auto [ x, y ] = GetMousePositionImpl();
		return ( float )y;
	}
}
