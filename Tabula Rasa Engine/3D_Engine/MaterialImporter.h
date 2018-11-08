#ifndef __MATERIAL_IMPORTER_H__
#define __MATERIAL_IMPORTER_H__

#include "Importer.h"
#include "trDefs.h"
#include <string>

class Texture;

class MaterialImporter : public Importer
{
public:

	MaterialImporter();
	~MaterialImporter();

	bool Import(const char* path, std::string& output_file);
	bool Import(const void* buffer, uint size, std::string& output_file);

	Texture* LoadImageFromPath(const char* path);

	void DeleteTextureBuffer(Texture* tex);

};

#endif // __MATERIAL_IMPORTER_H__
