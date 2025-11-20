#pragma once

#include "Rendering/Renderer.h"
#include "Scene/Scene.h"
#include <memory>

namespace MikuEngine
{
	class Layer
	{
	public:
		Layer() = default;
		virtual ~Layer() = default;

		virtual void Update( double dt ) {};
		virtual void Render( const Renderer& renderer ) const {};
		virtual void RenderImgui() {};

		template <typename TScene>
			requires( std::is_base_of_v<Scene, TScene> )
		void PushScene()
		{
			m_Scene = std::make_unique<TScene>();
		}

	protected:
	protected:
		std::unique_ptr<Scene> m_Scene;
	};
}
