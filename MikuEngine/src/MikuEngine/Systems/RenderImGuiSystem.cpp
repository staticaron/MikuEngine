#include "Systems/RenderImGuiSystem.h"

#include <string>

#include "glm/glm.hpp"

#include "AppLevelStuff.h"
#include "Components/Transform.h"
#include "Managers/ImguiManager.h"

namespace MikuEngine
{
	void RenderImGuiSystem::RenderImGui( entt::registry& registry, const AppLevelStuff& appLevelStuff )
	{
		auto transforms = registry.view<TransformComponent>();

		for ( const auto& entity : transforms )
		{
			auto& transformComponent = registry.get<TransformComponent>( entity );

			std::string windowName = "Transform##" + std::to_string( entt::entt_traits<entt::entity>::to_entity( entity ) );

			ImGui::Begin( windowName.c_str() );

			ImGui::DragFloat3( "Position", &transformComponent.Position[ 0 ] );
			ImGui::DragFloat3( "Rotation", &transformComponent.Rotation[ 0 ] );
			ImGui::DragFloat3( "Scale", &transformComponent.Scale[ 0 ] );

			registry.patch<TransformComponent>( entity );

			auto& transformLookup = registry.get<TransformComponent>( entity );

			DISABLED_IMGUI( ImGui::DragFloat3( "View Position", &transformLookup.Position[ 0 ] ) );
			DISABLED_IMGUI( ImGui::DragFloat3( "View Rotation", &transformLookup.Rotation[ 0 ] ) );
			DISABLED_IMGUI( ImGui::DragFloat3( "View Scale", &transformLookup.Scale[ 0 ] ) );

			ImGui::End();
		}
	}
}
