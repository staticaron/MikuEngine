#pragma once

#include <filesystem>
#include <vector>

#include "assimp/scene.h"

#include "Core.h"
#include "IAsset.h"
#include "Rendering/Primitives/Mesh.h"
#include "UUID.h"

namespace MikuEngine
{
	class MIKU_API Model : public IAsset
	{
	public:
		Model() : IAsset( AssetType::MODEL ) {}
		Model( const std::filesystem::path& filepath, UUID uuid = {} );

		void Load( const std::filesystem::path& filepath );

		std::vector<Mesh> GetMeshes() const { return m_Meshes; }

		const UUID& GetUUID() const override { return m_UUID; }
		std::string GetName() const override { return m_FilePath.stem().string(); }
		const std::filesystem::path& GetPath() const override { return m_FilePath; }
		void SetPath( const std::filesystem::path& newPath ) override { m_FilePath = newPath; }

	private:
		void ProcessNode( const aiScene* scene, aiNode* node );
		Mesh ProcessMesh( const aiScene* scene, aiMesh* mesh );

	private:
		UUID m_UUID;
		std::filesystem::path m_FilePath;

		std::vector<Mesh> m_Meshes;
	};
}
