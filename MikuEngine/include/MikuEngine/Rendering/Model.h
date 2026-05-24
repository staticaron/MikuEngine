#pragma once

#include <filesystem>
#include <vector>

#include "assimp/scene.h"

#include "Asset.h"
#include "Core.h"
#include "Rendering/Primitives/Mesh.h"
#include "UUID.h"

namespace MikuEngine
{
	class MIKU_API Model : public Asset
	{
	public:
		Model() : Asset( AssetType::MODEL ) {}
		Model( UUID uuid ) : m_UUID( uuid ), Asset( AssetType::MODEL ) {}

		void LoadFromFile( const std::filesystem::path& filepath );

		void ProcessNode( const aiScene* scene, aiNode* node );
		Mesh ProcessMesh( const aiScene* scene, aiMesh* mesh );

		std::vector<Mesh> GetMeshes() const { return m_Meshes; }

		void DeleteAsset() override;

		std::string GetName() const override;
		void SetName( const std::string& newName ) override;

		const std::filesystem::path& GetPath() const override;

	public:
	private:
		UUID m_UUID;

		std::vector<Mesh> m_Meshes;
	};
}
