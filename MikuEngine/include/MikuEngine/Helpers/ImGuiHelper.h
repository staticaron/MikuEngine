#pragma once

#include <functional>
#include <optional>

#include "Core.h"
#include "UUID.h"

namespace MikuEngine
{
	class MIKU_API ImGuiHelper
	{
	public:
		static bool RenderDragableTextureInput( const std::string& identifier, std::optional<UUID>& textureUUID, std::function<void()> textureEditBtnCallback );
		static bool RenderDragableModelInput( const std::string& identifier, std::optional<UUID>& modelUUID, std::function<void()> modelEditBtnCallback );
		static bool RenderDragableShaderInput( const std::string& identifier, std::optional<UUID>& shaderUUID, std::function<void()> shaderEditBtnCallback );
		static bool RenderDragableMaterialInput( const std::string& identifier, std::optional<UUID>& materialUUID, std::function<void()> materialEditBtnCallback );

		static void StartPropertyTable();
		static void EndPropertyTable();

		static void RenderLabel( const char* label );
		static void RenderTableItem( const char* label );
		static void RenderTableItem( const char* label, std::function<void()> itemFunc );
	};
}
