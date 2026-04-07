#include "Input/CentralInput.h"

#include "Application.h"

namespace MikuEngine
{
	void CentralInput::Update( double dt )
	{
		UpdateMousePosition();
		UpdateMouseButtons();

		UpdateKeyboardButtons();
		UpdateAxisRaw();

		auto pos = GetAxisRaw();
	}

	void CentralInput::UpdateKeyboardButtons()
	{
		auto* window = Application::GetApplication()->GetWindow();

		m_KeyCodeState[ GLFW_KEY_W ] = glfwGetKey( window, GLFW_KEY_W );
		m_KeyCodeState[ GLFW_KEY_S ] = glfwGetKey( window, GLFW_KEY_S );
		m_KeyCodeState[ GLFW_KEY_D ] = glfwGetKey( window, GLFW_KEY_D );
		m_KeyCodeState[ GLFW_KEY_A ] = glfwGetKey( window, GLFW_KEY_A );

		m_KeyCodeState[ GLFW_KEY_SPACE ] = glfwGetKey( window, GLFW_KEY_SPACE );
	}

	void CentralInput::UpdateAxisRaw()
	{
		bool rightPressed = IsKeyPressed( GLFW_KEY_D );
		bool leftPressed = IsKeyPressed( GLFW_KEY_A );
		bool upPressed = IsKeyPressed( GLFW_KEY_W );
		bool downPressed = IsKeyPressed( GLFW_KEY_S );

		m_AxisRaw.x = rightPressed - leftPressed;
		m_AxisRaw.y = downPressed - upPressed;
	}

	void CentralInput::UpdateMousePosition()
	{
		m_LastMousePosition = m_MousePosition;
		glfwGetCursorPos( Application::GetApplication()->GetWindow(), &m_MousePosition.x, &m_MousePosition.y );
	}

	void CentralInput::UpdateMouseButtons()
	{
		m_MouseKeyCodeState[ GLFW_MOUSE_BUTTON_1 ] = glfwGetMouseButton( Application::GetApplication()->GetWindow(), GLFW_MOUSE_BUTTON_1 );
		m_MouseKeyCodeState[ GLFW_MOUSE_BUTTON_2 ] = glfwGetMouseButton( Application::GetApplication()->GetWindow(), GLFW_MOUSE_BUTTON_2 );
		m_MouseKeyCodeState[ GLFW_MOUSE_BUTTON_3 ] = glfwGetMouseButton( Application::GetApplication()->GetWindow(), GLFW_MOUSE_BUTTON_3 );
	}

	bool CentralInput::IsMouseButtonPressed( int keycode ) const
	{
		const auto& existing = m_MouseKeyCodeState.find( keycode );

		MIKU_ASSERT( existing != m_KeyCodeState.end(), "MOUSE BUTTON NOT FOUND!" );

		return existing->second == GLFW_PRESS || existing->second == GLFW_REPEAT;
	};

	bool CentralInput::IsKeyPressed( int keycode )
	{
		const auto& existing = m_KeyCodeState.find( keycode );

		MIKU_ASSERT( existing != m_KeyCodeState.end(), "KEY NOT FOUND!" );

		return existing->second == GLFW_PRESS || existing->second == GLFW_REPEAT;
	}
}
