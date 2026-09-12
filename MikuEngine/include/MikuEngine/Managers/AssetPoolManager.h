#pragma once

#include "Core.h"
#include "Managers/MaterialManager.h"
#include "Managers/ModelManager.h"
#include "Managers/ParticleEmitterManager.h"
#include "Managers/SceneManager.h"
#include "Managers/ShaderManager.h"
#include "Managers/TextureManager.h"

namespace MikuEngine
{
	class MIKU_API AssetPoolManager
	{
	public:
		void Init();
		void InitFrame();

		AssetType GetAssetTypeFromPool( const UUID& uuid ) const;

		TextureManager& GetTextureManager() { return m_TextureManager; }
		const TextureManager& GetTextureManager() const { return m_TextureManager; }

		ShaderManager& GetShaderManager() { return m_ShaderManager; }
		const ShaderManager& GetShaderManager() const { return m_ShaderManager; }

		MaterialManager& GetMaterialManager() { return m_MaterialManager; }
		const MaterialManager& GetMaterialManager() const { return m_MaterialManager; }

		ModelManager& GetModelManager() { return m_ModelManager; }
		const ModelManager& GetModelManager() const { return m_ModelManager; }

		SceneManager& GetSceneManager() { return m_SceneManager; }
		const SceneManager& GetSceneManager() const { return m_SceneManager; }

		ParticleEmitterManager& GetParticleEmitterManager() { return m_ParticleEmitterManager; }
		const ParticleEmitterManager& GetParticleEmitterManager() const { return m_ParticleEmitterManager; }

		AssetType GetAssetTypeFromFileExtension( const std::string& extension ) const;

	private:
		TextureManager m_TextureManager;
		ShaderManager m_ShaderManager;
		MaterialManager m_MaterialManager;
		ModelManager m_ModelManager;
		SceneManager m_SceneManager;
		ParticleEmitterManager m_ParticleEmitterManager;
	};
}
