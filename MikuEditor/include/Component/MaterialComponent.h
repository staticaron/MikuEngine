#pragma once

#include "Component/BaseComponentInspector.h"
#include "MikuEngine/Rendering/Material.h"

namespace MikuEditor
{
	class EditorLayer;
};

namespace MikuEditor
{
	class MaterialComponent : public BaseComponentInspector
	{
	public:
		static void RenderMaterialComponent( EditorLayer& editorLayer, MikuEngine::Material& material );
	};
}
