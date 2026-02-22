#include "Windows/ShaderSelectionWindow.h"

#include "imgui.h"

#include "MikuEngine/AppLevelStuff.h"
#include "MikuEngine/Components.h"
#include "MikuEngine/Entity.h"
#include "MikuEngine/Scene/Scene.h"

namespace MikuEditor
{
	ShaderSelectionWindow::ShaderSelectionWindow( MikuEngine::UUID uuid ) : m_EntityUUID( uuid ) {}

	WindowResponse ShaderSelectionWindow::RenderShaderSelectionWindow( const MikuEngine::AppLevelStuff& appLevelstuff, MikuEngine::Scene& scene )
	{
		WindowResponse response;

		ImGui::Begin( "Select Shader", &m_IsOpen );

		auto shaders = appLevelstuff.GetAssetPoolManager().GetShaderManager().GetAllLoadedShaders();

		for ( const auto& [ uuid, shaderContainer ] : shaders )
		{
			if ( ImGui::Selectable( shaderContainer.shaderDetails.path.c_str() ) )
			{
				auto entity = scene.GetEntityByID( m_EntityUUID );

				if ( entity.has_value() == false )
				{
					response = WindowResponse::ERROR;
					break;
				}

				entity->GetComponent<MikuEngine::SpriteRendererComponent>().ShaderUUID = uuid;
				response = WindowResponse::COMPLETED;
			}
		}

		ImGui::End();

		if ( m_IsOpen == false ) response = WindowResponse::CLOSED;

		return response;
	}
}
