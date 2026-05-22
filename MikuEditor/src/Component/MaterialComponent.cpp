#include "Component/MaterialComponent.h"

#include "Application.h"
#include "Layers/EditorLayer.h"
#include "MikuEngine/Application.h"
#include "MikuEngine/Helpers/ImGuiHelper.h"

namespace MikuEditor
{
	void MaterialComponent::RenderMaterialComponent( EditorLayer& editorLayer, MikuEngine::MaterialContainer& materialContainer )
	{
		ComponentHeader( [ materialContainer ]() { return materialContainer.GetName(); }, [ materialContainer ]( std::string newName ) { MikuEngine::Application::GetAppLevelStuff().GetAssetPoolManager().GetMaterialManager().RenameAsset( materialContainer.index.uuid, newName ); } );

		auto currentlySelected = materialContainer.material.GetBlendModeString();
		const char* blendModes[]{ "Transparent", "Opaque" };

		if ( ImGui::BeginCombo( "Blend Mode", currentlySelected.c_str() ) )
		{
			if ( ImGui::Selectable( "Transparent" ) )
			{
				materialContainer.material.SetRenderOrderMode( MikuEngine::MaterialBlendMode::TRANSPARENT );
			}
			if ( ImGui::Selectable( "Opaque" ) )
			{
				materialContainer.material.SetRenderOrderMode( MikuEngine::MaterialBlendMode::OPAQUE );
			}
			ImGui::EndCombo();
		}

		ImGui::DragScalar( "Render Order", ImGuiDataType_U32, &materialContainer.material.GetRenderOrder().order, 1 );

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

		MikuEngine::ImGuiHelper::RenderDragableShaderInput( "Shader", shaderUUID, shaderEditBtnCallback );

		// If no shader is attached then no need to render the shader properties
		if ( shaderUUID.has_value() )
		{
			// Render Textures
			//
			//
			auto textures = materialContainer.material.GetTextures();
			for ( auto& [ uniformName, uuid ] : textures )

			{
				std::optional<MikuEngine::UUID> texture = uuid;

				std::function<void( MikuEngine::UUID )> onTextureSelection = [ materialUUID, uniformName ]( MikuEngine::UUID selectedTextureUUID ) {
					auto materialSearch = MikuEngine::Application::GetAppLevelStuff().GetAssetPoolManager().GetMaterialManager().GetMaterial( materialUUID );
					if ( materialSearch.has_value() == false )
					{
						MIKU_CLIENT_WARN( "The item for which this window was opened no longer exists!" );
						return;
					}
					materialSearch.value()->material.SetTexture( uniformName, selectedTextureUUID );
				};

				std::function<void()> textureEditBtnCallback = [ &editorLayer, &onTextureSelection ]() { editorLayer.m_TextureSelectionWindow.emplace_back( onTextureSelection ); };

				bool wasChanged = MikuEngine::ImGuiHelper::RenderDragableTextureInput( uniformName, texture, textureEditBtnCallback );

				if ( wasChanged ) materialContainer.material.SetTexture( uniformName, texture.value() );
			}

			// Render Cubemap
			//
			//
			auto cubemaps = materialContainer.material.GetCubemaps();
			for ( auto& [ uniformName, uuid ] : cubemaps )
			{
				std::optional<MikuEngine::UUID> cubemap = uuid;

				std::function<void( MikuEngine::UUID )> onCubemapSelection = [ materialUUID, uniformName ]( MikuEngine::UUID selectedCubemapUUID ) {
					auto materialSearch = MikuEngine::Application::GetAppLevelStuff().GetAssetPoolManager().GetMaterialManager().GetMaterial( materialUUID );
					if ( materialSearch.has_value() == false )
					{
						MIKU_CLIENT_WARN( "The item for which this window was opened no longer exists!" );
						return;
					}
					materialSearch.value()->material.SetCubemap( uniformName, selectedCubemapUUID );
				};

				std::function<void()> cubemapEditBtnCallback = [ &editorLayer, &onCubemapSelection ]() { editorLayer.m_TextureSelectionWindow.emplace_back( onCubemapSelection ); };

				bool wasChanged = MikuEngine::ImGuiHelper::RenderDragableTextureInput( uniformName, cubemap, cubemapEditBtnCallback );

				if ( wasChanged ) materialContainer.material.SetCubemap( uniformName, cubemap.value() );
			}

			// Render Floats
			//
			//
			auto& floats = materialContainer.material.GetFloats();

			for ( auto& [ uniformName, value ] : floats )
			{
				float newValue = value;
				if ( ImGui::DragFloat( uniformName.c_str(), &newValue ) )
				{
					materialContainer.material.SetFloat( uniformName, newValue );
				}
			}
		}

		ImGui::Separator();
		if ( MikuEngine::ImguiManager::FullWidthButton( "SAVE" ) )
		{
			materialContainer.material.SaveToFile( materialContainer.material.GetPath() );
		}

		ComponentFooter( [ materialContainer ]() { MikuEngine::Application::GetAppLevelStuff().GetAssetPoolManager().GetMaterialManager().AddToDeleteQueue( materialContainer.index.uuid ); } );
	}
}
