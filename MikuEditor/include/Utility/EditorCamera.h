#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

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

		void Translate( double dt );

		void RenderImGui( EditorLevelStuff& editorLevelStuff );

	private:
		glm::vec3 Position{ 0.0f };
		glm::vec3 Rotation{ 0.0f };

		glm::mat4 m_ViewMatrix = {};
		glm::mat4 m_ProjectionMatrix = {};

		float m_RotationSpeed = 1.0f;

		float m_Pitch = 0.0f;
		float m_Yaw = 0.0f;
		glm::vec3 m_Forward = { 0.0f, 0.0f, -1.0f };

		float m_CameraSpeed = 10.0f;

		bool m_IsPerspective = true;

		float m_Zoom;

		bool m_ShowImGui = true;
	};
}
