#pragma once

#include "Core.h"

#include "Data/UniformBuffer/CameraUniformData.h"
#include "Data/UniformBuffer/LightingUniformData.h"

#include "Rendering/UniformBuffers/EditorCameraUniformBuffer.h"
#include "Rendering/UniformBuffers/GameCameraUniformBuffer.h"
#include "Rendering/UniformBuffers/LightingUniformBuffer.h"

namespace MikuEngine
{
	class MIKU_API UniformBufferManager
	{
	public:
		void Init();

		const UniformBuffer& GetGameUniformBuffer() const { return m_GameCameraUniformBuffer; }
		const UniformBuffer& GetEditorUniformBuffer() const { return m_EditorCameraUniformBuffer; }
		const UniformBuffer& GetLightingUniformBuffer() const { return m_LightingUniformBuffer; }

		void UpdateBuffer();

		void UpdateGameCameraData( const CameraUniformData& cameraData );
		void UpdateEditorCameraData( const CameraUniformData& cameraData );
		void UpdateLightingData( const LightingUniformData& lightingData );

	private:
		GameCameraUniformBuffer m_GameCameraUniformBuffer;
		EditorCameraUniformBuffer m_EditorCameraUniformBuffer;

		LightingUniformBuffer m_LightingUniformBuffer;

		CameraUniformData m_GameMatrixData;
		CameraUniformData m_EditorMatrixData;

		LightingUniformData m_LightingData;
	};
}
