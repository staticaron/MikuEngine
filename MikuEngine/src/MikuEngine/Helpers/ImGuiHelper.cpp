#include "Helpers/ImGuiHelper.h"

#include "Application.h"

namespace MikuEngine
{
	bool ImGuiHelper::RenderDragableTextureInput( const std::string& identifier, std::optional<UUID>& textureUUID, std::function<void()> textureEditBtnCallback )
	{
		bool wasChanged = false;

		const auto& textureManager = Application::GetAppLevelStuff().GetAssetPoolManager().GetTextureManager();

		std::string textureName = "<NONE>";

		if ( textureUUID.has_value() )
		{
			auto texture = textureManager.GetTexture( textureUUID.value() );
			if ( texture.has_value() )
				textureName = texture.value()->GetName();
		}

		ImGui::PushID( identifier.c_str() );

		std::string name = std::format( "Texture ( {} )", identifier.c_str() );

		ImGuiHelper::RenderTableItem( name.c_str(), [ & ]() {
			DISABLED_IMGUI( ImGui::Button( textureName.c_str() ) );
			ImGui::SameLine();

			if ( ImGui::Button( "EDIT..." ) )
				textureEditBtnCallback();

			if ( ImGui::BeginDragDropTarget() )
			{
				auto payload = ImGui::AcceptDragDropPayload( "TEXTURE_DRAG_DROP_PAYLOAD" );

				if ( payload != nullptr )
				{
					auto texturePath = static_cast<const char*>( payload->Data );
					auto texture = textureManager.GetTextureByFilePath( texturePath );
					if ( texture.has_value() )
					{
						wasChanged = true;
						textureUUID = texture.value()->GetUUID();
					}
				}

				ImGui::EndDragDropTarget();
			}
		} );

		ImGui::PopID();

		return wasChanged;
	}

	bool ImGuiHelper::RenderDragableModelInput( const std::string& identifier, std::optional<UUID>& modelUUID, std::function<void()> modelEditBtnCallback )
	{
		bool wasChanged = false;

		auto& modelManager = Application::GetAppLevelStuff().GetAssetPoolManager().GetModelManager();

		std::string modelName = "<NONE>";

		if ( modelUUID.has_value() )
		{
			auto model = modelManager.GetModel( modelUUID.value() );
			if ( model.has_value() )
				modelName = model.value()->index.GetName();
		}

		ImGui::PushID( identifier.c_str() );

		ImGuiHelper::RenderTableItem( "Model", [ & ]() {
			DISABLED_IMGUI( ImGui::Button( modelName.c_str() ) );
			ImGui::SameLine();

			if ( ImGui::Button( "EDIT..." ) )
				modelEditBtnCallback();

			if ( ImGui::BeginDragDropTarget() )
			{
				auto payload = ImGui::AcceptDragDropPayload( "MODEL_DRAG_DROP_PAYLOAD" );

				if ( payload != nullptr )
				{
					auto modelPath = static_cast<const char*>( payload->Data );
					auto modelContainer = modelManager.GetModelByFilePath( modelPath );
					modelUUID = modelManager.GetModelByFilePath( modelPath ).value()->index.uuid;
					wasChanged = true;
				}

				ImGui::EndDragDropTarget();
			}
		} );

		ImGui::PopID();

		return wasChanged;
	}

	// Renders a Property where the shader can be changed by either selecting from the shader selection window or dropping the shader itself.
	// This only modifies the shaderUUID container provided as parameter.
	bool ImGuiHelper::RenderDragableShaderInput( const std::string& identifier, std::optional<UUID>& shaderUUID, std::function<void()> shaderEditBtnCallback )
	{
		bool wasChanged = false;

		auto& shaderManager = Application::GetAppLevelStuff().GetAssetPoolManager().GetShaderManager();

		std::string shaderName = "<NONE>";

		if ( shaderUUID.has_value() )
		{
			auto shader = shaderManager.GetShader( shaderUUID.value() );
			if ( shader != nullptr )
				shaderName = shader->GetName();
		}

		ImGui::PushID( identifier.c_str() );

		ImGuiHelper::RenderTableItem( "Shader", [ & ]() {
			DISABLED_IMGUI( ImGui::Button( shaderName.c_str() ) );
			ImGui::SameLine();

			if ( ImGui::Button( "EDIT..." ) )
				shaderEditBtnCallback();

			if ( ImGui::BeginDragDropTarget() )
			{
				auto payload = ImGui::AcceptDragDropPayload( "SHADER_DRAG_DROP_PAYLOAD" );

				if ( payload != nullptr )
				{
					auto shaderPath = static_cast<const char*>( payload->Data );
					if ( auto shader = shaderManager.GetShaderByFilePath( shaderPath ); shader != nullptr )
					{
						wasChanged = true;
						shaderUUID = shader->GetUUID();
					}
				}

				ImGui::EndDragDropTarget();
			}
		} );

		ImGui::PopID();

		return wasChanged;
	};

	bool ImGuiHelper::RenderDragableMaterialInput( const std::string& identifier, std::optional<UUID>& materialUUID, std::function<void()> materialEditBtnCallback )
	{
		bool wasChanged = false;

		auto& materialManager = Application::GetAppLevelStuff().GetAssetPoolManager().GetMaterialManager();

		std::string materialName = "<NONE>";

		if ( materialUUID.has_value() )
		{
			auto material = materialManager.GetMaterial( materialUUID.value() );
			if ( material.has_value() )
				materialName = material.value()->GetName();
		}

		ImGui::PushID( identifier.c_str() );

		ImGuiHelper::RenderTableItem( "Material", [ & ]() {
			DISABLED_IMGUI( ImGui::Button( materialName.c_str() ) );
			ImGui::SameLine();

			if ( ImGui::Button( "EDIT..." ) )
				materialEditBtnCallback();

			if ( ImGui::BeginDragDropTarget() )
			{
				auto payload = ImGui::AcceptDragDropPayload( "MATERIAL_DRAG_DROP_PAYLOAD" );

				if ( payload != nullptr )
				{
					auto materialPath = static_cast<const char*>( payload->Data );
					auto material = materialManager.GetMaterialByFilePath( materialPath );
					if ( material.has_value() )
					{
						wasChanged = true;
						materialUUID = material.value()->GetUUID();
					}
				}

				ImGui::EndDragDropTarget();
			}
		} );

		ImGui::PopID();

		return wasChanged;
	};

	void ImGuiHelper::StartPropertyTable()
	{
		ImGui::SetCursorPosX( ImGui::GetCursorPosX() + 8.0f );
		ImGui::BeginTable( "##Transform", 2 );

		ImGui::TableSetupColumn( "Property Name", ImGuiTableColumnFlags_WidthStretch, 0.3 );
		ImGui::TableSetupColumn( "Property Value", ImGuiTableColumnFlags_WidthStretch, 0.7 );
	}

	void ImGuiHelper::EndPropertyTable()
	{
		ImGui::EndTable();
	}

	void ImGuiHelper::RenderLabel( const char* label )
	{
		ImGui::AlignTextToFramePadding();
		ImGui::TextUnformatted( label );
		ImGui::SameLine();
	}

	void ImGuiHelper::RenderLabel( const char* label, std::function<void()> itemFunc )
	{
		ImGui::AlignTextToFramePadding();
		ImGui::TextUnformatted( label );
		ImGui::SameLine();
	}

	void ImGuiHelper::RenderTableItem( const char* label ) {}

	void ImGuiHelper::RenderTableItem( const char* label, std::function<void()> itemFunc )
	{
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex( 0 );
		ImGui::TextUnformatted( label );

		ImGui::TableSetColumnIndex( 1 );
		ImGui::SetNextItemWidth( ImGui::GetContentRegionAvail().x );
		itemFunc();
	}
}
