#pragma once

#include "glm/glm.hpp"

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

	public:
		bool m_IsMainCamera = true;
		float Zoom = 1.0f;
		glm::vec2 m_Resolution = { 1600, 900 };
	};
}
