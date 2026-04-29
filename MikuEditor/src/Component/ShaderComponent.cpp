#include "Component/ShaderComponent.h"

#include "MikuEngine/Application.h"

namespace MikuEditor
{
	void ShaderComponent::RenderShaderComponent( MikuEngine::ShaderContainer& shaderContainer )
	{
		ComponentHeader( [ shaderContainer ]() { return shaderContainer.GetName(); }, [ shaderContainer ]( std::string newName ) { MikuEngine::Application::GetAppLevelStuff().GetAssetPoolManager().GetShaderManager().RenameAsset( shaderContainer.index.uuid, newName ); } );

		ComponentFooter( [ shaderContainer ]() { MikuEngine::Application::GetAppLevelStuff().GetAssetPoolManager().GetShaderManager().AddToDeleteQueue( shaderContainer.index.uuid ); } );
	}
}
