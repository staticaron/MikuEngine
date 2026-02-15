#include "Rendering/Primitives/Quad.h"

#include "Rendering/Vertex.h"

namespace MikuEngine
{
	std::array<Vertex, 4> Quad::m_Verts;
	std::array<unsigned int, 6> Quad::m_Indices;

	void Quad::Init()
	{
		SetupVertsAndIndices();

		m_VB.Init( 4 * sizeof( Vertex ), nullptr );
		m_IB.Init( 6, nullptr );

		auto verts = GetVerts();

		m_VB.PutData( verts.data(), GetVerts().size() * sizeof( Vertex ) );
		m_IB.PutData( GetIndices().data(), GetIndices().size() );

		m_VBL.Add<float>( 3 );
		m_VBL.Add<float>( 2 );

		m_VA.Init();
		m_VA.Setup( m_VB, m_VBL );
	}

	void Quad::SetupVertsAndIndices()
	{
		// clang-format off
		m_Verts[ 0 ] = {{ -0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f } };
		m_Verts[ 1 ] = { { 0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f } };
		m_Verts[ 2 ] = { { 0.5f, 0.5f, 0.0f }, { 1.0f, 0.0f } };
		m_Verts[ 3 ] = {{ -0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f } };
		// clang-format on

		m_Indices = { 0, 1, 2, 2, 3, 0 };
	}

	const std::array<Vertex, 4>& Quad::GetVerts()
	{
		return m_Verts;
	}

	std::array<unsigned int, 6> Quad::GetIndices()
	{
		return m_Indices;
	}
}
