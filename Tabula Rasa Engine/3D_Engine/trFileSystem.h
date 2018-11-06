#ifndef __trFILESYSTEM_H__
#define __trFILESYSTEM_H__

#include "trModule.h"

struct PHYSFS_File;


class trFileSystem : public trModule
{
public:
	trFileSystem();
	~trFileSystem();

	bool Awake(JSON_Object* config = nullptr);
	bool Start();
	bool CleanUp();

	bool DoesFileExist(const char* file_name) const;
	bool AddNewPath(const char* path);

	PHYSFS_File* OpenFileForWriting(const char* file_name) const;
	PHYSFS_File* OpenFileForReading(const char* file_name) const;
	void CloseFile(PHYSFS_File* file, const char* file_name) const;

	bool WriteInFile(const char* file_name, char* buffer, uint size) const;
	bool ReadFromFile(const char* file_name, char* buffer);

	bool MakeNewDir(const char* dir_name);
		
private:

};

#endif // __trFILESYSTEM_H__