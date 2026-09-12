#include "Component/ShaderComponent.h"

#include "MikuEngine/Application.h"

namespace MikuEditor
{
	void ShaderComponent::RenderShaderComponent( MikuEngine::Shader& shader )
	{
		ComponentHeader( [ shader ]() { return shader.GetName(); }, [ shader ]( std::string newName ) { MikuEngine::Application::GetAppLevelStuff().GetAssetPoolManager().GetShaderManager().AddToRenameQueue( shader.GetUUID(), newName ); } );
		ComponentFooter( [ shader ]() { MikuEngine::Application::GetAppLevelStuff().GetAssetPoolManager().GetShaderManager().AddToDeleteQueue( shader.GetUUID() ); } );
	}
}
