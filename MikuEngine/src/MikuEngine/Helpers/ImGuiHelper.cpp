#include "Helpers/ImGuiHelper.h"

#include "Application.h"

namespace MikuEngine
{
	void ImGuiHelper::RenderDragableTextureInput( std::optional<UUID>& textureUUID, std::function<void()> textureEditBtnCallback )
	{
		const auto& textureManager = Application::GetAppLevelStuff().GetAssetPoolManager().GetTextureManager();

		std::string textureName = "<NONE>";

		if ( textureUUID.has_value() )
		{
			auto texture = textureManager.GetTexture( textureUUID.value() );
			if ( texture.has_value() ) textureName = texture.value()->GetName();
		}

		ImGui::Text( "Texture" );
		ImGui::SameLine();
		DISABLED_IMGUI( ImGui::Button( textureName.c_str() ) );
		ImGui::SameLine();
		if ( ImGui::Button( "EDIT...##Texture" ) ) textureEditBtnCallback();

		if ( ImGui::BeginDragDropTarget() )
		{
			auto payload = ImGui::AcceptDragDropPayload( "TEXTURE_DRAG_DROP_PAYLOAD" );

			if ( payload != nullptr )
			{
				auto texturePath = static_cast<const char*>( payload->Data );
				auto texture = textureManager.GetTextureByFilePath( texturePath );
				if ( texture.has_value() ) textureUUID = texture.value()->index.uuid;
			}

			ImGui::EndDragDropTarget();
		}
	}

	void ImGuiHelper::RenderDragableModelInput( std::optional<UUID>& modelUUID, std::function<void()> modelEditBtnCallback )
	{
		const auto& modelManager = Application::GetAppLevelStuff().GetAssetPoolManager().GetModelManager();

		std::string modelName = "<NONE>";

		if ( modelUUID.has_value() )
		{
			auto model = modelManager.GetModel( modelUUID.value() );
			modelName = model.index.Name;
		}

		ImGui::PushID( "Model" );

		ImGui::Text( "Model" );
		ImGui::SameLine();

		DISABLED_IMGUI( ImGui::Button( modelName.c_str() ) );
		ImGui::SameLine();

		if ( ImGui::Button( "EDIT..." ) ) modelEditBtnCallback();

		if ( ImGui::BeginDragDropTarget() )
		{
			auto payload = ImGui::AcceptDragDropPayload( "MODEL_DRAG_DROP_PAYLOAD" );

			if ( payload != nullptr )
			{
				auto modelPath = static_cast<const char*>( payload->Data );
				modelUUID = modelManager.GetModelByFilePath( modelPath ).index.uuid;
			}

			ImGui::EndDragDropTarget();
		}

		ImGui::PopID();
	}

	void ImGuiHelper::RenderDragableMaterialInput( std::optional<UUID>& materialUUID, std::function<void()> materialEditBtnCallback )
	{
		auto& materialManager = Application::GetAppLevelStuff().GetAssetPoolManager().GetMaterialManager();

		std::string materialName = "<NONE>";

		if ( materialUUID.has_value() )
		{
			auto material = materialManager.GetMaterial( materialUUID.value() );
			if ( material.has_value() ) materialName = material.value()->GetName();
		}

		ImGui::PushID( "Material" );

		ImGui::Text( "Material" );
		ImGui::SameLine();

		DISABLED_IMGUI( ImGui::Button( materialName.c_str() ) );
		ImGui::SameLine();

		if ( ImGui::Button( "EDIT...##material" ) ) materialEditBtnCallback();

		if ( ImGui::BeginDragDropTarget() )
		{
			auto payload = ImGui::AcceptDragDropPayload( "MATERIAL_DRAG_DROP_PAYLOAD" );

			if ( payload != nullptr )
			{
				auto materialPath = static_cast<const char*>( payload->Data );
				auto material = materialManager.GetMaterialByFilePath( materialPath );
				if ( material.has_value() ) materialUUID = material.value()->GetUUID();
			}

			ImGui::EndDragDropTarget();
		}

		ImGui::PopID();
	};
}
