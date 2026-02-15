#pragma once

#include <array>

#include "Core.h"
#include "Primitive.h"

namespace MikuEngine
{
	class Vertex;
}

namespace MikuEngine
{
	class MIKU_API Quad : public Primitive
	{
	public:
		void Init() override;
		void SetupVertsAndIndices() override;

		static const std::array<Vertex, 4>& GetVerts();
		static std::array<unsigned int, 6> GetIndices();

	private:
		static std::array<Vertex, 4> m_Verts;
		static std::array<unsigned int, 6> m_Indices;
	};
} // namespace MikuEngine
