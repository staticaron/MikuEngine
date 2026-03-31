#pragma once

#include <filesystem>
#include <vector>

#include "assimp/scene.h"

#include "Core.h"
#include "Rendering/Primitives/Mesh.h"

namespace MikuEngine
{
	class MIKU_API Model
	{
	public:
		void LoadFromFile( const std::filesystem::path& filepath );

		void ProcessNode( const aiScene* scene, aiNode* node );
		Mesh ProcessMesh( const aiScene* scene, aiMesh* mesh );

		std::vector<Mesh> GetMeshes() const { return m_Meshes; }

	public:
	private:
		std::vector<Mesh> m_Meshes;
	};
}
