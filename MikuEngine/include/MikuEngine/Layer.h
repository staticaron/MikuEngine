#pragma once

#include <memory>

#include "AppLevelStuff.h"
#include "Managers/TextureManager.h"
#include "Rendering/Renderer.h"
#include "Scene/Scene.h"

namespace MikuEngine
{
	class Layer
	{
	public:
		Layer( AppLevelStuff& appLevelStuff ) : m_AppLevelStuff( appLevelStuff ) {}
		virtual ~Layer() {};
		virtual void Update( double dt ) = 0;
		virtual void Render( const Renderer& renderer, const TextureManager& textureManager ) const = 0;
		virtual void RenderImgui() = 0;

		template <typename TScene>
			requires( std::is_base_of_v<Scene, TScene> )
		void PushScene()
		{
			m_Scenes.push_back( std::make_unique<TScene>() );
		}

	protected:
	protected:
		std::vector<std::unique_ptr<Scene>> m_Scenes;

		AppLevelStuff& m_AppLevelStuff;
	};
}
