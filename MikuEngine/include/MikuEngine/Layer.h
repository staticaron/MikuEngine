#pragma once

#include <memory>

#include "Core.h"

#include "AppLevelStuff.h"
#include "Scene/Scene.h"

namespace MikuEngine
{
	class MIKU_API Layer
	{
	public:
		virtual ~Layer() {};
		virtual void Update( double dt ) = 0;
		virtual void Render( AppLevelStuff& appLevelStuff ) const = 0;
		virtual void RenderImgui( const AppLevelStuff& appLevelStuff ) = 0;

		template <typename TScene>
			requires( std::is_base_of_v<Scene, TScene> )
		void PushScene()
		{
			m_Scenes.push_back( std::make_unique<TScene>() );
		}

	protected:
	protected:
		std::vector<std::unique_ptr<Scene>> m_Scenes;
	};
}
