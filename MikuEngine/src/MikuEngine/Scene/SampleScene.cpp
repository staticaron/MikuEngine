#include "Scene/SampleScene.h"
#include "Managers/TextureManager.h"
#include "Rendering/Renderer.h"

#include "Components/SpriteRenderer.h"
#include "Components/Transform.h"

#include "Systems/RenderingSystem.h"

namespace MikuEngine
{
	SampleScene::SampleScene()
	{
		auto entitiy = m_Registry.create();

		m_Registry.emplace<TransformComponent>( entitiy );
		m_Registry.emplace<SpriteRendererComponent>( entitiy );
	}

	void SampleScene::Update( double dt ) {}

	void SampleScene::Render( const Renderer& renderer, const TextureManager& textureManager ) const
	{
		// RenderSprite( m_Registry );
	}

	void SampleScene::RenderImGui() {}
}
