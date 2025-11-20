#include "GameObject.h"

namespace MikuEngine
{
	void GameObject::Render( const Renderer& renderer ) const
	{
		m_SpriteRenderer.Render( renderer );
	}
}
