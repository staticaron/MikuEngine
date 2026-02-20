#pragma once

#include "Core.h"

#include "Rendering/Material.h"
#include "UUID.h"

namespace MikuEngine
{
	class MIKU_API MaterialManager
	{
	public:
	private:
		std::unordered_map<UUID, Material> m_Materials;
	};
}
