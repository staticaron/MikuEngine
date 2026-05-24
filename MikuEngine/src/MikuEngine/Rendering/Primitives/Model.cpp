#include "Rendering/Model.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include "Application.h"
#include "Logger.h"

namespace MikuEngine
{
	void Model::LoadFromFile( const std::filesystem::path& filepath )
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile( filepath.c_str(), aiProcess_Triangulate | aiProcess_CalcTangentSpace );

		if ( !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode )
		{
			MIKU_CORE_ERROR( "Couldn't load the model @ ", filepath.string() );
			return;
		}

		ProcessNode( scene, scene->mRootNode );
	}

	void Model::ProcessNode( const aiScene* scene, aiNode* node )
	{
		// Process the meshes attached to this node
		for ( unsigned int x = 0; x < node->mNumMeshes; x++ )
		{
			aiMesh* mesh = scene->mMeshes[ node->mMeshes[ x ] ];
			m_Meshes.push_back( ProcessMesh( scene, mesh ) );
		}

		// Process the child nodes of this node
		for ( unsigned int y = 0; y < node->mNumChildren; y++ )
		{
			ProcessNode( scene, node->mChildren[ y ] );
		}
	}

	Mesh Model::ProcessMesh( const aiScene* scene, aiMesh* mesh )
	{
		std::vector<Vertex3D> vertices;
		std::vector<unsigned int> indices;

		// Load all the vertices
		for ( unsigned int x = 0; x < mesh->mNumVertices; x++ )
		{
			Vertex3D vertex;

			vertex.Position = { mesh->mVertices[ x ].x, mesh->mVertices[ x ].y, mesh->mVertices[ x ].z };
			vertex.Normal = { mesh->mNormals[ x ].x, mesh->mNormals[ x ].y, mesh->mNormals[ x ].z };

			vertex.Tangent = { mesh->mTangents[ x ].x, mesh->mTangents[ x ].y, mesh->mTangents[ x ].z };
			vertex.BiTangent = { mesh->mBitangents[ x ].x, mesh->mBitangents[ x ].y, mesh->mBitangents[ x ].z };

			if ( mesh->mTextureCoords[ 0 ] )
				vertex.UV = { mesh->mTextureCoords[ 0 ][ x ].x, mesh->mTextureCoords[ 0 ][ x ].y };
			else
				vertex.UV = { 0, 0 };

			vertices.push_back( vertex );
		}

		// Load all the indices
		for ( unsigned int y = 0; y < mesh->mNumFaces; y++ )
		{
			aiFace& face = mesh->mFaces[ y ];
			for ( unsigned int z = 0; z < face.mNumIndices; z++ )
				indices.push_back( face.mIndices[ z ] );
		}

		Mesh newMesh( vertices, indices );
		newMesh.Init();

		return newMesh;
	}

	void Model::DeleteAsset()
	{
		MIKU_CORE_WARN( "Deleting Model!!" );
		Application::GetAppLevelStuff().GetAssetPoolManager().GetMaterialManager().AddToDeleteQueue( m_UUID );
	}

	const std::filesystem::path& Model::GetPath() const
	{
		auto model = Application::GetAppLevelStuff().GetAssetPoolManager().GetModelManager().GetModel( m_UUID );
		MIKU_ASSERT( model.has_value(), "This Shader with UUID doesn't exists!" );
		return model.value()->index.path;
	}

	std::string Model::GetName() const
	{
		return GetPath().stem().string();
	}

	void Model::SetName( const std::string& newName )
	{
		Application::GetAppLevelStuff().GetAssetPoolManager().GetModelManager().RenameAsset( m_UUID, newName );
	}

}
