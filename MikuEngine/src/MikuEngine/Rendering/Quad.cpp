#include "Rendering/Quad.h"

namespace MikuEngine
{
	Quad::Quad( const std::string& shaderPath ) : m_VB( 4 * sizeof( Vertex ), nullptr ), m_IB( 6 * sizeof( unsigned int ), nullptr )
	{
		m_VB.PutData( GetVerts().data(), GetVerts().size() * sizeof( Vertex ) );
		m_IB.PutData( GetIndices().data(), GetIndices().size() * sizeof( unsigned int ) );

		m_VBL.Add<float>( 3 );
		m_VBL.Add<float>( 2 );

		m_VA.Setup( m_VB, m_VBL );

		m_ShaderPath = shaderPath == "" ? m_ShaderPath : shaderPath;
		m_Shader.LoadFromFile( m_ShaderPath );
	}

	std::array<Vertex, 4> Quad::GetVerts() const
	{
		std::array<Vertex, 4> verts;

		// clang-format off
		verts[ 0 ] = { { -0.5f, -0.5f, 0.0f }, { 0,0 } };
		verts[ 1 ] = { {  0.5f, -0.5f, 0.0f }, { 1,0 } };
		verts[ 2 ] = { {  0.5f,  0.5f, 0.0f }, { 1,1 } };
		verts[ 3 ] = { { -0.5f,  0.5f, 0.0f }, { 0,1 } };
		// clang-format on

		return verts;
	}

	std::array<unsigned int, 6> Quad::GetIndices() const
	{
		return { 0, 1, 2, 2, 3, 0 };
	}
}
