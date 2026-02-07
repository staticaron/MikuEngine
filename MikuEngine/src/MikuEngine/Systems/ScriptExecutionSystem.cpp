#include "Systems/ScriptExecutionSystem.h"

#include "MikuEngine/Components/DataComponent.h"
#include "MikuEngine/Components/IDComponent.h"
#include "MikuEngine/Components/NativeScriptComponent.h"
#include "MikuEngine/Scene/Scene.h"

namespace MikuEngine
{
	void ScriptExecutionSystem::ExecuteScripts( Scene& scene )
	{
		auto nscEntities = scene.GetRegistry().view<IDComponent, NativeScriptComponent>();

		for ( auto [ entity, idC, nsC ] : nscEntities.each() )
		{
			auto name = scene.GetRegistry().get<DataComponent>( entity ).EntityName;

			if ( nsC.Instance == nullptr ) nsC.Instantiate();

			auto entt = scene.GetEntityByID( idC.ID );

			if ( entt.has_value() == false ) continue;

			nsC.OnUpdate( entt.value() );
		}
	}
}
