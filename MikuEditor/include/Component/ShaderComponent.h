#pragma once

#include "BaseComponentInspector.h"
#include "MikuEngine/Rendering/Shader.h"

namespace MikuEditor
{
	class ShaderComponent : public BaseComponentInspector
	{
	public:
		static void RenderShaderComponent( MikuEngine::Shader& shaderContainer );
	};
}
