#include "Managers/UniformBufferManager.h"

#include "glad/glad.h"

#include "Logger.h"

namespace MikuEngine
{
	void UniformBufferManager::Init()
	{
		m_GameUniformBuffer.Init( 0, sizeof( MatrixData ) );
		m_EditorUniformBuffer.Init( 0, sizeof( MatrixData ) );

		m_LightingUniformBuffer.Init( 1, sizeof( LightingData ) );
	}

	void UniformBufferManager::UpdateGameMatrixData( const MatrixData& matrixData )
	{
		bool result = matrixData == m_GameMatrixData;
		if ( result ) return;

		m_GameMatrixData = matrixData;
		m_GameUniformBuffer.PutData( &m_GameMatrixData, sizeof( MatrixData ) );
	}

	void UniformBufferManager::UpdateEditorMatrixData( const MatrixData& matrixData )
	{
		if ( matrixData == m_EditorMatrixData ) return;

		m_EditorMatrixData = matrixData;
		m_EditorUniformBuffer.PutData( &m_EditorMatrixData, sizeof( MatrixData ) );
	}

	void UniformBufferManager::UpdateLightingData( const LightingData& lightingData )
	{
		if ( lightingData == m_LightingData ) return;

#define TESTJ
#ifdef TEST
		MIKU_CORE_INFO( "Intensity is {}", lightingData.intensity );

		LightingData oldData;

		m_LightingUniformBuffer.Bind();
		glGetBufferSubData( GL_UNIFORM_BUFFER, 0, sizeof( oldData ), &oldData );
		MIKU_CORE_INFO( "Intesity Before Change : {}", oldData.intensity );
		m_LightingUniformBuffer.UnBind();
#endif

		m_LightingData = lightingData;
		m_LightingUniformBuffer.PutData( &m_LightingData, sizeof( LightingData ) );

#define TEST_AFTERJ
#ifdef TEST_AFTER
		m_LightingUniformBuffer.Bind();
		glGetBufferSubData( GL_UNIFORM_BUFFER, 0, sizeof( oldData ), &oldData );
		MIKU_CORE_INFO( "Intensity After Change : {}", oldData.intensity );
#endif

		m_LightingUniformBuffer.UnBind();
	}
}
