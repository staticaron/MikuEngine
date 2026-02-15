#include "Rendering/Primitives/Cube.h"

#include "Rendering/Vertex.h"

namespace MikuEngine
{
	std::array<Vertex, 24> Cube::m_Verts;
	std::array<unsigned int, 36> Cube::m_Indices;

	void Cube::Init()
	{
		SetupVertsAndIndices();

		m_VB.Init( 8 * sizeof( Vertex ), nullptr );
		m_IB.Init( 36, nullptr );

		m_VB.PutData( GetVerts().data(), GetVerts().size() * sizeof( Vertex ) );
		m_IB.PutData( GetIndices().data(), GetIndices().size() );

		m_VBL.Add<float>( 3 );
		m_VBL.Add<float>( 2 );

		m_VA.Init();
		m_VA.Setup( m_VB, m_VBL );
	}

	void Cube::SetupVertsAndIndices()
	{
		// clang-format off
		m_Verts[ 0] = {{ -0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f } };
		m_Verts[ 1] = {{  0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f } };
		m_Verts[ 2] = {{  0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f } };
		m_Verts[ 3] = {{ -0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f } };

		m_Verts[ 4] = {{  0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f } };
		m_Verts[ 5] = {{  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f } };
		m_Verts[ 6] = {{  0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f } };
		m_Verts[ 7] = {{  0.5f, -0.5f,  0.5f }, { 0.0f, 1.0f } };

		m_Verts[ 8] = {{  0.5f,  0.5f,  0.5f }, { 0.0f, 0.0f } };
		m_Verts[ 9] = {{ -0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f } };
		m_Verts[10] = {{ -0.5f, -0.5f,  0.5f }, { 1.0f, 1.0f } };
		m_Verts[11] = {{  0.5f, -0.5f,  0.5f }, { 0.0f, 1.0f } };

		m_Verts[12] = {{ -0.5f,  0.5f,  0.5f }, { 0.0f, 0.0f } };
		m_Verts[13] = {{ -0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f } };
		m_Verts[14] = {{ -0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f } };
		m_Verts[15] = {{ -0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f } };

		m_Verts[16] = {{  0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f } };
		m_Verts[17] = {{  0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f } };
		m_Verts[18] = {{ -0.5f, -0.5f,  0.5f }, { 1.0f, 1.0f } };
		m_Verts[19] = {{ -0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f } };

		m_Verts[20] = {{ -0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f } };
		m_Verts[21] = {{ -0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f } };
		m_Verts[22] = {{  0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f } };
		m_Verts[23] = {{  0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f } };

		m_Indices = { 
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4,
			8, 9, 10, 10, 11, 8,
			12, 13, 14, 14, 15, 12,
			16, 17, 18, 18, 19, 16,
			20, 21, 22, 22, 23, 20,
		};
		// clang-format on
	}

	std::array<Vertex, 24> Cube::GetVerts()
	{
		return m_Verts;
	}

	std::array<unsigned int, 36> Cube::GetIndices()
	{
		return m_Indices;
	}
}
