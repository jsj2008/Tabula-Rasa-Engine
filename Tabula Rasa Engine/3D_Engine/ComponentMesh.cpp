#include "ComponentMesh.h"

#include "trApp.h"
#include "trResources.h"
#include "trRenderer3D.h"
#include "trFileLoader.h"
#include "SceneImporter.h"
#include "GameObject.h"
#include "Resource.h"
#include "ResourceMesh.h"
#include "ResourceBone.h"

#include "ComponentBone.h"

ComponentMesh::ComponentMesh(GameObject * embedded_game_object) : 
	Component(embedded_game_object, Component::component_type::COMPONENT_MESH)
{
}

ComponentMesh::ComponentMesh(GameObject * embedded_game_object, UID resource) : 
	Component(embedded_game_object, Component::component_type::COMPONENT_MESH)
{
	this->resource = resource;
}

ComponentMesh::~ComponentMesh()
{
	Resource* res = (Resource*)GetResource();
	if(res)
		res->Release();
}

bool ComponentMesh::Save(JSON_Object* component_obj) const
{
	//todo: get resource path etc
	const Resource* res = this->GetResource();
	json_object_set_string(component_obj, "path", res->GetExportedFile());
	return true;
}

bool ComponentMesh::Load(const JSON_Object * component_obj)
{
	bool ret = true;

	JSON_Value* value = json_object_get_value(component_obj, "path");
	const char* file_path = json_value_get_string(value);

	//todo clean
	if (file_path) {
		std::string uid_force = file_path;
		const size_t last_slash = uid_force.find_last_of("\\/");
		if (std::string::npos != last_slash)
			uid_force.erase(0, last_slash + 1);
		const size_t extension = uid_force.rfind('.');
		if (std::string::npos != extension)
			uid_force.erase(extension);
		UID uid = static_cast<unsigned int>(std::stoul(uid_force));

		SetResource(App->resources->mesh_importer->GenerateResourceFromFile(file_path, uid));
	}

	return ret;
}

bool ComponentMesh::SetResource(UID resource)
{
	if (Resource* res = (Resource*)GetResource()) {
		res->Release();
	}

	this->resource = resource;
	ResourceMesh* mesh_res = (ResourceMesh*)this->GetResource();
	
	if(mesh_res)
		uint num_references = mesh_res->LoadToMemory();

	if (mesh_res) {
		embedded_go->bounding_box = AABB(float3(0.f, 0.f, 0.f), float3(0.f, 0.f, 0.f));
		embedded_go->bounding_box.Enclose((float3*)mesh_res->vertices, mesh_res->vertex_size / 3);
	}

	return true;
}

void ComponentMesh::AttachBones(const GameObject* go)
{
	std::vector<ComponentBone*> bones;
	RecursiveFindBones(go, bones);

	if (bones.size() > 0)
	{
		DetachBones();
		root_bones = go;
		attached_bones = bones;

		ResourceMesh* res = (ResourceMesh*)this->GetResource();

		if (res->deformable == nullptr)
		{
			res->deformable = new ResourceMesh(0); //Check this
			//res->deformable->CreateDeformableVersion((const ResourceMesh*)GetResource());
			//App->meshes->GenerateVertexBuffer(deformable);
		}

		for (std::vector<ComponentBone*>::iterator it = attached_bones.begin(); it != attached_bones.end(); ++it)
			(*it)->attached_mesh = this;
	}
}

// ---------------------------------------------------------
void ComponentMesh::DetachBones()
{
	for (std::vector<ComponentBone*>::iterator it = attached_bones.begin(); it != attached_bones.end(); ++it)
		(*it)->attached_mesh = nullptr;
	attached_bones.clear();

	ResourceMesh* res = (ResourceMesh*)this->GetResource();
	RELEASE(res->deformable);
}

// ---------------------------------------------------------
uint ComponentMesh::CountAttachedBones() const
{
	return attached_bones.size();
}

void ComponentMesh::RecursiveFindBones(const GameObject * go, std::vector<ComponentBone*>& output) const
{
	if (go == nullptr)
		return;

	for (std::list<Component*>::const_iterator it = go->components.begin(); it != go->components.end(); ++it)
	{
		if ((*it)->GetType() == Component::component_type::COMPONENT_BONE)
		{
			ComponentBone* bone = (ComponentBone*)*it;
			ResourceBone* res = (ResourceBone*)bone->GetResource();

			if (res != nullptr && res->mesh_uid == GetResourceUID())
			{
				output.push_back(bone);
			}
		}
	}

	for (std::list<GameObject*>::const_iterator it = go->childs.begin(); it != go->childs.end(); ++it)
		RecursiveFindBones(*it, output);
}