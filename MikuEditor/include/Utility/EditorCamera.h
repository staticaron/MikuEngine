#pragma once

#include "glm/gtc/matrix_transform.hpp"

namespace MikuEditor
{
	class EditorLevelStuff;
}

namespace MikuEditor
{
	class EditorCamera
	{
	public:
		EditorCamera() = default;
		~EditorCamera() = default;

		glm::mat4 GetMVPFromModelMatrix( glm::mat4 modelMatrix ) const { return GetProjViewMatrix() * modelMatrix; }

		glm::mat4 GetProjViewMatrix() const { return GetProjMatrix() * GetViewMatrix(); }

		glm::mat4 GetProjMatrix() const;
		glm::mat4 GetViewMatrix() const;

		void Translate( double dt );

		void RenderImGui( EditorLevelStuff& editorLevelStuff );

	private:
		glm::vec3 Position{ 0.0f };
		glm::vec3 Rotation{ 0.0f };

		float m_CameraSpeed = 10.0f;

		bool m_IsPerspective = true;

		float m_Zoom;

		bool m_ShowImGui = true;
	};
}
