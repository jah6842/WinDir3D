#include "Folder.h"

// Constructors
Folder::Folder(){
	parent = nullptr;
	name = "UNNAMED_FOLDER";
	totalSize = 0;
	localSize = 0;
	position = glm::vec3(0.0f,0.0f,0.0f);
	direction = glm::vec4(0.0f,1.0f,0.0f,0.0f);
	radius = 0.0f;
	angle = 360.0f;
}
Folder::Folder(std::string n){
	parent = nullptr;
	name = n.c_str();
	totalSize = 0;
	localSize = 0;
	position = glm::vec3(0.0f,0.0f,0.0f);
	direction = glm::vec4(0.0f,1.0f,0.0f,0.0f);
	radius = 0.0f;
	angle = 360.0f;
}
Folder::Folder(Folder* par, std::string n){
	parent = par;
	name = n.c_str();
	totalSize = 0;
	localSize = 0;
	position = glm::vec3(0.0f,0.0f,0.0f);
	direction = glm::vec4(0.0f,1.0f,0.0f,0.0f);
	radius = 0.0f;
	angle = 360.0f;
}
// destructor
Folder::~Folder(){
	// Nothing to explicitly destroy.
}

void Folder::AddFile(std::string name, File* file){
	files.insert(std::pair<std::string, File*>(name, file));
	localSize += file->size;
	PropagateFileSize(file->size);
}
void Folder::PropagateFileSize(__int64 size){
	this->totalSize += size;
	if(parent != nullptr)
		parent->PropagateFileSize(size);
}

std::string Folder::GetPath(Folder* folder){
	Folder* curFolder = folder;
	std::string temp = "";
	temp = curFolder->name + "\\";

	while(curFolder->parent != nullptr){
		temp.insert(0, curFolder->parent->name + "\\");
		curFolder = curFolder->parent;
	}

	return temp;
}

Folder* Folder::GetNextFolder(int curFolder){
	if(parent != nullptr){
		int count = -1;

		for(std::unordered_map<std::string, Folder*>::iterator itr = parent->subfolders.begin(); itr != parent->subfolders.end(); itr++){
			count++;
			if(count <= curFolder)
				continue;

			return itr->second;
		}

	}

	return nullptr;
}

Folder* Folder::GetPreviousFolder(int curFolder){

	if(parent != nullptr){
		int count = parent->subfolders.size();

		for(std::unordered_map<std::string, Folder*>::iterator itr = parent->subfolders.end(); itr != parent->subfolders.begin(); itr--){
			count--;
			if(count >= curFolder)
				continue;

			return itr->second;
		}

	}

	return nullptr;
}