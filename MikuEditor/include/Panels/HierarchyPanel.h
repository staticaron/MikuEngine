#pragma once

#include "MikuEngine/Core.h"
#include "UUID.h"

namespace MikuEngine
{
	class Scene;
	class Entity;
	class UUID;
}

namespace MikuEditor
{
	class EditorLayer;
}

namespace MikuEditor
{
	class MIKU_API HierarchyPanel
	{
	public:
		static void RenderHierarchy( EditorLayer& editorLayer, MikuEngine::Scene& scene );

	private:
		static void RenderNode( EditorLayer& editorLayer, MikuEngine::Scene& scene, std::unordered_map<MikuEngine::UUID, std::vector<MikuEngine::UUID>> parentChildren, MikuEngine::UUID currentUUID );
	};
}
