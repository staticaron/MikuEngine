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
		static void RenderDragableTextureInput( const std::string& identifier, std::optional<UUID>& textureUUID, std::function<void()> textureEditBtnCallback );
		static void RenderDragableModelInput( const std::string& identifier, std::optional<UUID>& modelUUID, std::function<void()> modelEditBtnCallback );
		static void RenderDragableShaderInput( const std::string& identifier, std::optional<UUID>& shaderUUID, std::function<void()> shaderEditBtnCallback );
		static void RenderDragableMaterialInput( const std::string& identifier, std::optional<UUID>& materialUUID, std::function<void()> materialEditBtnCallback );
	};
}
