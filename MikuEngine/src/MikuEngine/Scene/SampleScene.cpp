#include "Scene/SampleScene.h"
#include "AppLevelStuff.h"

#include "Components/SpriteRenderer.h"
#include "Components/Transform.h"

namespace MikuEngine
{
	SampleScene::SampleScene()
	{
		auto entitiy = m_Registry.create();

		m_Registry.emplace<TransformComponent>( entitiy );
		m_Registry.emplace<SpriteRendererComponent>( entitiy );
	}

	void SampleScene::Update( double dt ) {}

	void SampleScene::Render( AppLevelStuff& appLevelStuff ) const
	{
		// RenderSprite( m_Registry );
	}

	void SampleScene::RenderImGui() {}
}
