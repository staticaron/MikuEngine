#include "Input/WindowsInput.h"

#include "Application.h"
#include "Input/Input.h"
#include "Logger.h"

namespace MikuEngine
{
	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl( int keycode )
	{
		auto window = Application::GetApplication()->GetWindow();
		auto state = glfwGetKey( window, keycode );
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	std::pair<int, int> WindowsInput::GetAxisRawImpl()
	{
		bool rightPressed = Input::IsKeyPressed( 68 );
		bool leftPressed = Input::IsKeyPressed( 65 );
		bool upPressed = Input::IsKeyPressed( 87 );
		bool downPressed = Input::IsKeyPressed( 83 );

		int xAxis = 0, yAxis = 0;

		xAxis = rightPressed - leftPressed;
		yAxis = downPressed - upPressed;

		return { xAxis, yAxis };
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

		return { xPos - 64, yPos - 47 };
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
