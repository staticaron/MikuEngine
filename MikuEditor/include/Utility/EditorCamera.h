#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

#include "Logger.h"

namespace MikuEditor
{
	class EditorLevelStuff;
}

namespace MikuEditor
{
	static constexpr glm::vec3 WORLD_UP = { 0.0f, 1.0f, 0.0f };

	class EditorCamera
	{
	public:
		EditorCamera();
		~EditorCamera() = default;

		void UpdateProjectionMatrix();
		void UpdateViewMatrix();

		void Update( double dt );

		glm::mat4 GetMVPFromModelMatrix( glm::mat4 modelMatrix ) const { return GetProjViewMatrix() * modelMatrix; }

		glm::mat4 GetProjViewMatrix() const { return GetProjMatrix() * GetViewMatrix(); }
		glm::mat4 GetProjMatrix() const { return m_ProjectionMatrix; }
		glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }

		glm::quat GetOrientation() const { return glm::quat( glm::vec3( -m_Pitch, -m_Yaw, 0.0f ) ); }
		glm::vec3 GetForwardDirection() const { return glm::rotate( GetOrientation(), glm::vec3( 0.0f, 0.0f, -1.0f ) ); }
		glm::vec3 GetRightDirection() const { return glm::rotate( GetOrientation(), glm::vec3( 1.0f, 0.0f, 0.0f ) ); }
		glm::vec3 GetUpDirection() const { return glm::rotate( GetOrientation(), glm::vec3( 0.0f, 1.0f, 0.0f ) ); }

		const glm::vec3& GetPosition() const { return Position; }

		void Translate( double dt );

		void RenderImGui( EditorLevelStuff& editorLevelStuff );

	private:
		glm::vec3 Position{ 0.0f };

		glm::mat4 m_ViewMatrix = {};
		glm::mat4 m_ProjectionMatrix = {};

		float m_RotationSpeed = 0.0005f;

		float m_Pitch = 0.0f;
		float m_Yaw = 0.0f;

		bool m_MovementLocked = true;

		float m_CameraSpeed = 20.0f;

		float m_Zoom;

		bool m_ShowImGui = true;
	};
}
