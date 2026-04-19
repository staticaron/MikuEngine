#include "Managers/UniformBufferManager.h"

namespace MikuEngine
{
	void UniformBufferManager::Init()
	{
		m_GameCameraUniformBuffer.Init( 0, sizeof( CameraUniformData ) );
		m_EditorCameraUniformBuffer.Init( 0, sizeof( CameraUniformData ) );

		m_LightingUniformBuffer.Init( 1, sizeof( LightingUniformData ) );
	}

	void UniformBufferManager::UpdateGameCameraData( const CameraUniformData& matrixData )
	{
		bool result = matrixData == m_GameMatrixData;
		if ( result ) return;

		m_GameMatrixData = matrixData;
		m_GameCameraUniformBuffer.PutData( &m_GameMatrixData, sizeof( CameraUniformData ) );
	}

	void UniformBufferManager::UpdateEditorCameraData( const CameraUniformData& matrixData )
	{
		if ( matrixData == m_EditorMatrixData ) return;

		m_EditorMatrixData = matrixData;
		m_EditorCameraUniformBuffer.PutData( &m_EditorMatrixData, sizeof( CameraUniformData ) );
	}

	void UniformBufferManager::UpdateLightingData( const LightingUniformData& lightingData )
	{
		if ( lightingData == m_LightingData ) return;

		m_LightingData = lightingData;
		m_LightingUniformBuffer.PutData( &m_LightingData, sizeof( LightingUniformData ) );

		m_LightingUniformBuffer.UnBind();
	}
}
