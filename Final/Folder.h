#ifndef _FOLDER_H
#define _FOLDER_H

#include <unordered_map>
#include <string>
#include "File.h"
#include "glm\glm.hpp"

class Folder {
public:
	Folder* parent;
	std::string name;
	__int64 totalSize;
	__int64 localSize;
	std::unordered_map<std::string, Folder*> subfolders;
	std::unordered_map<std::string, File*> files;

	glm::vec3 position;
	glm::vec4 direction;
	float angle;
	float radius;
	
	// Constructors
	Folder();
	Folder(std::string n);
	Folder(Folder* par, std::string n);
	~Folder();
	void AddFile(std::string name, File* file);
	void PropagateFileSize(__int64 size);

	Folder* GetNextFolder(int curFolder);
	Folder* GetPreviousFolder(int curFolder);

	static std::string GetPath(Folder* folder);
};

#endif // _FOLDER_H