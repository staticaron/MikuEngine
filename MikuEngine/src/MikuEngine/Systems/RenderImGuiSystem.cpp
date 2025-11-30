#include "Systems/RenderImGuiSystem.h"

#include <string>

#include "glm/glm.hpp"

#include "AppLevelStuff.h"
#include "Components/DataComponent.h"
#include "Components/TransformComponent.h"

namespace MikuEngine
{
	void RenderImGuiSystem::RenderImGui( entt::registry& registry, const AppLevelStuff& appLevelStuff )
	{
		auto entities = registry.view<DataComponent, TransformComponent>();

		for ( const auto& [ entity, data, transform ] : entities.each() )
		{
			auto& transformComponent = registry.get<TransformComponent>( entity );

			std::string windowName = data.EntityName + " - Transform##" + std::to_string( entt::entt_traits<entt::entity>::to_entity( entity ) );

			ImGui::Begin( windowName.c_str() );

			ImGui::DragFloat3( "Position", &transformComponent.Position[ 0 ] );
			ImGui::DragFloat3( "Rotation", &transformComponent.Rotation[ 0 ] );
			ImGui::DragFloat3( "Scale", &transformComponent.Scale[ 0 ] );

			registry.patch<TransformComponent>( entity );

			ImGui::End();
		}
	}
}
