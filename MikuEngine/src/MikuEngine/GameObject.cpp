#include "GameObject.h"

#include "Managers/TextureManager.h"

namespace MikuEngine
{
	void GameObject::Render( const Renderer& renderer, const TextureManager& textureManager, const Camera& camera ) const
	{
		m_SpriteRenderer.Render( renderer, textureManager, camera );
	}
}
