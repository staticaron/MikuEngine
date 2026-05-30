#pragma once

#include "MikuEngine/Layer.h"

#include "EditorLevelStuff.h"
#include "Panels/AssetBrowserPanel.h"
#include "Panels/ViewportPanel.h"
#include "Utility/EditorCamera.h"
#include "Windows/MaterialSelectionWindow.h"
#include "Windows/ModelSelectionWindow.h"
#include "Windows/ShaderSelectionWindow.h"
#include "Windows/TextureSelectionWindow.h"

namespace MikuEngine
{
	class AppLevelStuff;
}
namespace MikuEditor
{
	class InspectorPanel;
	class MaterialComponent;
}

namespace MikuEditor
{
	struct SelectableAsset
	{
		MikuEngine::AssetType assetType = MikuEngine::AssetType::NONE;
		MikuEngine::UUID uuid{ 0 };
		std::string assetPath = "";
	};

	class EditorLayer : public MikuEngine::Layer
	{
	public:
		EditorLayer();

		static EditorLayer* GetEditorLayer();

		void Update( double dt ) override;
		void Render( MikuEngine::AppLevelStuff& appLevelStuff ) const override;
		void RenderImgui( const MikuEngine::AppLevelStuff& appLevelStuff ) override;
		const EditorLevelStuff& GetEditorLayerInfo() const { return m_EditorLevelStuff; }

		EditorCamera& GetEditorCamera() { return m_EditorCamera; }

		void SetSeletedEntity( MikuEngine::UUID& entityUUID );
		void SetSeletedAsset( MikuEngine::AssetType assetType, MikuEngine::UUID assetUUID, const std::filesystem::path& assetPath );
		std::optional<SelectableAsset> GetSeletedAsset() { return m_SelectedAsset; }

	private:
		void ManageTextureSelectionWindows( const MikuEngine::AppLevelStuff& appLevelStuff );
		void ManageShaderSelectionWindows( const MikuEngine::AppLevelStuff& appLevelStuff );
		void ManageMaterialSelectionWindows( const MikuEngine::AppLevelStuff& appLevelStuff );
		void ManageModelSelectionWindows( const MikuEngine::AppLevelStuff& appLevelStuff );

	private:
		EditorLevelStuff m_EditorLevelStuff;
		EditorCamera m_EditorCamera;

		std::vector<TextureSelectionWindow> m_TextureSelectionWindow;
		std::vector<ShaderSelectionWindow> m_ShaderSelectionWindow;
		std::vector<MaterialSelectionWindow> m_MaterialSelectionWindow;
		std::vector<ModelSelectionWindow> m_ModelSelectionWindow;

		bool m_IsViewportPanelFocused = false;

		// Panels
		AssetBrowserPanel m_AssetBrowserPanel;
		ViewportPanel m_ViewportPanel;

		std::optional<SelectableAsset> m_SelectedAsset = std::nullopt;

		friend class InspectorPanel;
		friend class MaterialComponent;
	};
}
