#include "Component/MaterialComponent.h"

#include "Layers/EditorLayer.h"
#include "MikuEngine/Application.h"
#include "MikuEngine/Helpers/ImGuiHelper.h"

namespace MikuEditor
{
	void MaterialComponent::RenderMaterialComponent( EditorLayer& editorLayer, MikuEngine::MaterialContainer& materialContainer )
	{
		ComponentHeader( [ materialContainer ]() { return materialContainer.GetName(); }, [ materialContainer ]( std::string newName ) { MikuEngine::Application::GetAppLevelStuff().GetAssetPoolManager().GetMaterialManager().RenameAsset( materialContainer.index.uuid, newName ); } );

		auto& shaderManager = MikuEngine::Application::GetAppLevelStuff().GetAssetPoolManager().GetShaderManager();

		auto shader = materialContainer.material.GetShader();

		std::optional<MikuEngine::UUID> shaderUUID;
		if ( shader.has_value() ) shaderUUID = shader.value()->shader.GetUUID();

		MikuEngine::UUID materialUUID = materialContainer.index.uuid;
		std::function<void( MikuEngine::UUID itemUUID )> onShaderSelection = [ materialUUID ]( MikuEngine::UUID selectedShaderUUID ) {
			auto materialSearch = MikuEngine::Application::GetAppLevelStuff().GetAssetPoolManager().GetMaterialManager().GetMaterial( materialUUID );
			if ( materialSearch.has_value() == false )
			{
				MIKU_CLIENT_WARN( "The item for which this window was opened no longer exists!" );
				return;
			}
			materialSearch.value()->material.SetShader( selectedShaderUUID );
		};

		std::function<void()> shaderEditBtnCallback = [ &editorLayer, &shaderUUID, onShaderSelection ]() { editorLayer.m_ShaderSelectionWindow.emplace_back( onShaderSelection ); };

		MikuEngine::ImGuiHelper::RenderDragableShaderInput( shaderUUID, shaderEditBtnCallback );

		// If no shader is attached then no need to render the shader properties
		if ( shaderUUID.has_value() )
		{
			// Render Textures
			//
			//
			auto textures = materialContainer.material.GetTextures();

			for ( auto& [ uniformName, uuid ] : textures )
			{
				const auto& textureUUID = textures[ uniformName ];

				auto texture = MikuEngine::Application::GetAppLevelStuff().GetAssetPoolManager().GetTextureManager().GetTexture( uuid );

				std::string textureName = "<NONE>";
				if ( texture.has_value() ) textureName = texture.value()->GetName();

				char buff[ 256 ] = "";
				std::copy( textureName.begin(), textureName.end(), buff );
				buff[ textureName.length() ] = '\0';

				ImGui::InputText( uniformName.c_str(), buff, 256, ImGuiInputTextFlags_ReadOnly );

				if ( ImGui::BeginDragDropTarget() )
				{
					const ImGuiPayload* payload = ImGui::AcceptDragDropPayload( "TEXTURE_DRAG_DROP_PAYLOAD" );

					if ( payload != nullptr )
					{
						std::string filePath = static_cast<const char*>( payload->Data );
						std::filesystem::path materialPath = filePath;

						const auto& texture = MikuEngine::Application::GetAppLevelStuff().GetAssetPoolManager().GetTextureManager().GetTextureByFilePath( filePath );
						textures[ uniformName ] = texture.value()->index.uuid;
					}

					ImGui::EndDragDropTarget();
				}
			}

			// Render Floats
			//
			//
			auto floats = materialContainer.material.GetFloats();

			for ( auto& [ uniformName, value ] : floats )
			{
				ImGui::DragFloat( uniformName.c_str(), &floats[ uniformName ] );
			}
		}

		ComponentFooter( [ materialContainer ]() { MikuEngine::Application::GetAppLevelStuff().GetAssetPoolManager().GetMaterialManager().AddToDeleteQueue( materialContainer.index.uuid ); } );
	}
}
