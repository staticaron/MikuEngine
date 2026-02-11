#include "Systems/ScriptExecutionSystem.h"

#include "imgui.h"

#include "MikuEngine.h"
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
			if ( nsC.ScriptIdentifier == "" ) continue;

			auto entt = scene.GetEntityByID( idC.ID );
			if ( entt.has_value() == false ) continue;

			// Instantiate the Script Object -> If the script object was not instantiated! Do not execute scripts
			if ( nsC.Instance == nullptr )
				if ( nsC.Instantiate( entt.value() ) == false )
				{
					MIKU_CORE_WARN( "Script Instantiation Failed for Script {}. You have attached a NativeScript which does not exist in the GameLogicDLL", nsC.ScriptIdentifier );
					continue;
				}

			nsC.Update( Application::GetAppLevelStuff().GetDeltaTime() );
		}
	}

	void ScriptExecutionSystem::NativeScriptComponentRenderImGui( Entity entity, NativeScriptComponent& nativeScriptComponent )
	{
		bool keep = true;

		if ( ImGui::CollapsingHeader( "NativeScriptComponent", &keep ) )
		{
			char buff[ 128 ];

			std::copy( nativeScriptComponent.ScriptIdentifier.begin(), nativeScriptComponent.ScriptIdentifier.begin() + nativeScriptComponent.ScriptIdentifier.length(), buff );
			buff[ nativeScriptComponent.ScriptIdentifier.length() ] = '\0';

			ImGui::InputText( "Script", buff, 128 );

			if ( buff != nativeScriptComponent.ScriptIdentifier.c_str() ) nativeScriptComponent.ScriptIdentifier = buff;
		}

		if ( !keep ) entity.RemoveComponent<NativeScriptComponent>();
	}

	void ScriptExecutionSystem::SerializeNativeScriptComponent( const Entity& entity, YAML::Emitter& emitter )
	{
		emitter << YAML::BeginMap;

		auto nativeScript = entity.GetReadOnlyComponent<NativeScriptComponent>();
		emitter << YAML::Key << "type" << YAML::Value << "NativeScriptComponent";

		emitter << YAML::Key << "values" << YAML::Value << YAML::BeginMap;
		emitter << YAML::Key << "scriptName" << YAML::Value << nativeScript.ScriptIdentifier;
		emitter << YAML::EndMap;

		emitter << YAML::EndMap;
	}

	void ScriptExecutionSystem::DeSerializeNativeScriptComponent( NativeScriptComponent& nativeScriptC, const YAML::Node& node )
	{
		std::string scriptName = node[ "scriptName" ].as<std::string>();
		nativeScriptC.ScriptIdentifier = scriptName;
	}
}
