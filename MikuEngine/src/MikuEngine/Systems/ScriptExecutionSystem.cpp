#include "Systems/ScriptExecutionSystem.h"

#include "MikuEngine/Components/IDComponent.h"
#include "MikuEngine/Components/NativeScriptComponent.h"
#include "MikuEngine/Scene/Scene.h"
#include "MikuEngine/ScriptRegistry.h"

namespace MikuEngine
{
	void ScriptExecutionSystem::ExecuteScripts( Scene& scene )
	{
		auto nscEntities = scene.GetRegistry().view<IDComponent, NativeScriptComponent>();

		for ( auto [ entity, idC, nsC ] : nscEntities.each() )
		{
			if ( nsC.Instance == nullptr ) nsC.Instance = ScriptRegistry::RegisteredScripts.at( "SampleClass" ).CreatorFn();

			auto entt = scene.GetEntityByID( idC.ID );

			if ( entt.has_value() == false ) continue;

			nsC.Update( entt.value() );
		}
	}
}
