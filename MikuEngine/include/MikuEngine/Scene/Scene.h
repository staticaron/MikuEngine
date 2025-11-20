#pragma once

#include "Core.h"
#include "GameObject.h"
#include "Rendering/Renderer.h"

namespace MikuEngine
{
	class MIKU_API Scene
	{
	public:
		virtual void Update( double dt )
		{
		}

		virtual void Render( const Renderer& renderer ) const
		{
		}

		virtual void RenderImGui()
		{
		}

		virtual void AddGameObject( GameObject go )
		{
			m_GameObjects.push_back( go );
		}

	private:
	private:
		std::vector<GameObject> m_GameObjects;
	};
}
