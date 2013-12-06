#ifndef _FILE_H
#define _FILE_H

#include <string>
#include "glm\glm.hpp"

class File {
public:
	std::string name;
	__int64 size;

	glm::vec3 position;
	File(){
		name = "UNNAMED_FILE";
		size = -1;
		position = glm::vec3(0.0f,0.0f,0.0f);
	}
	File(std::string n, __int64 s){
		name = n;
		size = s;
		position = glm::vec3(0.0f,0.0f,0.0f);
	}
};

#endif // _FILE_H