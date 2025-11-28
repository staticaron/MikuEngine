#include "Rendering/Quad.h"

namespace MikuEngine
{
	Quad::Quad( const std::string& shaderPath )
	{
		m_ShaderPath = shaderPath == "" ? m_ShaderPath : shaderPath;
	}

	void Quad::Init()
	{
		m_VB.Init( 4 * sizeof( Vertex ), nullptr );
		m_IB.Init( 6, nullptr );

		m_VB.PutData( GetVerts().data(), GetVerts().size() * sizeof( Vertex ) );
		m_IB.PutData( GetIndices().data(), GetIndices().size() );

		m_VBL.Add<float>( 3 );
		m_VBL.Add<float>( 2 );

		m_VA.Init();
		m_VA.Setup( m_VB, m_VBL );

		// Load Base Shader if not provided in the constructor
		m_Shader.LoadFromFile( m_ShaderPath );
	}

	std::array<Vertex, 4> Quad::GetVerts()
	{
		std::array<Vertex, 4> verts;

		// clang-format off
		verts[ 0 ] = { { -0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f } };
		verts[ 1 ] = { {  0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f } };
		verts[ 2 ] = { {  0.5f,  0.5f, 0.0f }, { 1.0f, 0.0f } };
		verts[ 3 ] = { { -0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f } };
		// clang-format on

		return verts;
	}

	std::array<unsigned int, 6> Quad::GetIndices() const
	{
		return { 0, 1, 2, 2, 3, 0 };
	}
}
