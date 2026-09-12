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
		Model( const std::filesystem::path& filepath, UUID uuid = {} ) : IAsset( AssetType::MODEL ), m_UUID( uuid ), m_FilePath( filepath ) {}

		void LoadFromFile( const std::filesystem::path& filepath );

		void ProcessNode( const aiScene* scene, aiNode* node );
		Mesh ProcessMesh( const aiScene* scene, aiMesh* mesh );

		std::vector<Mesh> GetMeshes() const { return m_Meshes; }

		void DeleteAsset();

		std::string GetName() const override;
		void SetPath( const std::string& newPath ) { m_FilePath = newPath; }

		const std::filesystem::path& GetPath() const override;

	public:
	private:
		UUID m_UUID;
		std::filesystem::path m_FilePath;

		std::vector<Mesh> m_Meshes;
	};
}
