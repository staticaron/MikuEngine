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
		static void RenderDragableTextureInput( std::optional<UUID>& textureUUID, std::function<void()> textureEditBtnCallback );
		static void RenderDragableModelInput( std::optional<UUID>& modelUUID, std::function<void()> modelEditBtnCallback );
		static void RenderDragableMaterialInput( std::optional<UUID>& materialUUID, std::function<void()> materialEditBtnCallback );
	};
}
