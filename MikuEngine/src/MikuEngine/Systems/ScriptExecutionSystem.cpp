#include "Systems/ScriptExecutionSystem.h"

#include "imgui.h"

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

			if ( nsC.Instance == nullptr ) nsC.Instantiate();

			auto entt = scene.GetEntityByID( idC.ID );

			if ( entt.has_value() == false ) continue;

			nsC.Update( entt.value() );
		}
	}

	void ScriptExecutionSystem::NativeScriptComponentRenderImGui( NativeScriptComponent& nativeScriptComponent )
	{
		if ( ImGui::TreeNode( "NativeScriptComponent" ) )
		{
			char buff[ 128 ];

			std::copy( nativeScriptComponent.ScriptIdentifier.begin(), nativeScriptComponent.ScriptIdentifier.begin() + nativeScriptComponent.ScriptIdentifier.length(), buff );
			buff[ nativeScriptComponent.ScriptIdentifier.length() ] = '\0';

			ImGui::InputText( "Script", buff, 128 );

			if ( buff != nativeScriptComponent.ScriptIdentifier.c_str() ) nativeScriptComponent.ScriptIdentifier = buff;

			ImGui::TreePop();
		}
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

	void DeSerializeNativeScriptComponent( NativeScriptComponent& nativeScriptC, const YAML::Node& node )
	{
		std::string scriptName = node[ "scriptName" ].as<std::string>();
		nativeScriptC.ScriptIdentifier = scriptName;
	}
}
