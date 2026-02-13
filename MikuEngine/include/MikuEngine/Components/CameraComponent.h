#pragma once

#include "glm/glm.hpp"

#include "Application.h"
#include "Components/BaseComponent.h"
#include "Core.h"

namespace MikuEngine
{
	class MIKU_API CameraComponent : public BaseComponent
	{
	public:
		CameraComponent() = default;
		CameraComponent( const CameraComponent& ) = default;

		CameraComponent( float zoomLevel ) : Zoom( zoomLevel ) {}

		bool IsMainCamera() const { return m_IsMainCamera; }
		float GetZoom() const { return Zoom; }
		glm::vec2 GetCameraSize() const { return { m_CameraWidth, m_CameraWidth / Application::GetDataContainer().GetGameAspectRatio() }; }

	public:
		bool m_IsMainCamera = true;
		float Zoom = 1.0f;
		int m_CameraWidth = 30;
	};
}
