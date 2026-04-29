#pragma once

#include "BaseComponentInspector.h"
#include "Managers/ShaderManager.h"

namespace MikuEditor
{
	class ShaderComponent : public BaseComponentInspector
	{
	public:
		static void RenderShaderComponent( MikuEngine::ShaderContainer& shaderContainer );
	};
}
