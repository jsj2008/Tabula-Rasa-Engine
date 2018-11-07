#ifndef __MESH_IMPORTER_H__
#define __MESH_IMPORTER_H__

#include "Importer.h"
#include <vector>
#include <string>
#include "MathGeoLib/MathGeoLib.h"

class GameObject;
class ComponentMaterial;
class aiScene;
class aiNode;
class aiMaterial;
class Mesh;

class MeshImporter : public Importer
{
public:
	MeshImporter();
	~MeshImporter();


public:

	bool Import(const char* path, std::string& output_file);
	bool Import(const void* buffer, uint size, std::string& output_file);
	//bool Load(const char* exported_file, Texture* resource);

	void ImportNodesRecursively(const aiNode* node, const aiScene* scene, char* file_path);
	ComponentMaterial* LoadTexture(aiMaterial* material, GameObject* go);

	bool SaveMeshFile(const char* file_name, Mesh* mesh_data, std::string& output_file);
	bool LoadMeshFile(const char* file_path);

private:

	//used as static
	ComponentMaterial* material_data = nullptr;
	char* cursor_data = nullptr;

	// Tabula Rasa's format
	std::string file_name;

	std::vector<float3> scene_vertices;
	uint scene_num_vertex = 0;
	AABB model_bouncing_box;
	GameObject* model_root = nullptr;

};

#endif // __MESH_IMPORTER_H__