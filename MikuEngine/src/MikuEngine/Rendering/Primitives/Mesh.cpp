#include "Rendering/Primitives/Mesh.h"

namespace MikuEngine
{
	Mesh::Mesh( std::vector<Vertex3D> vertices, std::vector<unsigned int> indices )
	{
		m_Vertices = vertices;
		m_Indices = indices;
	}

	void Mesh::Init()
	{
		m_VB.Init( m_Vertices.size() * sizeof( Vertex3D ), nullptr );
		m_IB.Init( m_Indices.size(), nullptr );

		m_VB.PutData( m_Vertices.data(), m_Vertices.size() * sizeof( Vertex3D ) );
		m_IB.PutData( m_Indices.data(), m_Indices.size() );

		m_VBL.Add<float>( 3 );
		m_VBL.Add<float>( 3 );
		m_VBL.Add<float>( 2 );

		m_VA.Init();
		m_VA.Setup( m_VB, m_VBL );
	}

	void Mesh::SetupVertsAndIndices() {}
}
