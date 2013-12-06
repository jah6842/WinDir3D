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
	Folder(){
		parent = nullptr;
		name = "UNNAMED_FOLDER";
		totalSize = 0;
		localSize = 0;
		position = glm::vec3(0.0f,0.0f,0.0f);
		direction = glm::vec4(0.0f,1.0f,0.0f,0.0f);
		radius = 0.0f;
		angle = 360.0f;
	}
	Folder(std::string n){
		parent = nullptr;
		name = n.c_str();
		totalSize = 0;
		localSize = 0;
		position = glm::vec3(0.0f,0.0f,0.0f);
		direction = glm::vec4(0.0f,1.0f,0.0f,0.0f);
		radius = 0.0f;
		angle = 360.0f;
	}
	Folder(Folder* par, std::string n){
		parent = par;
		name = n.c_str();
		totalSize = 0;
		localSize = 0;
		position = glm::vec3(0.0f,0.0f,0.0f);
		direction = glm::vec4(0.0f,1.0f,0.0f,0.0f);
		radius = 0.0f;
		angle = 360.0f;
	}
	~Folder(){
		// Nothing to explicitly destroy.
	}
	void AddFile(std::string name, File* file){
		files.insert(std::pair<std::string, File*>(name, file));
		localSize += file->size;
		PropagateFileSize(file->size);
	}
	void PropagateFileSize(__int64 size){
		this->totalSize += size;
		if(parent != nullptr)
			parent->PropagateFileSize(size);
	}

	static std::string GetPath(Folder* folder){
		Folder* curFolder = folder;
		std::string temp = "";
		temp = curFolder->name + "\\";

		while(curFolder->parent != nullptr){
			temp.insert(0, curFolder->parent->name + "\\");
			curFolder = curFolder->parent;
	}

	return temp;
};
};

#endif // _FOLDER_H