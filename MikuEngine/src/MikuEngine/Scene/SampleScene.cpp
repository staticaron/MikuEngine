#include "Scene/SampleScene.h"
#include "AppLevelStuff.h"

#include "Components/SpriteRendererComponent.h"
#include "Components/TransformComponent.h"

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

	void SampleScene::RenderImGui( const AppLevelStuff& appLevelStuff ) {}
}
