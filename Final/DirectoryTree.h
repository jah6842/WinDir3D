#ifndef _DIRECTORY_TREE_H
#define _DIRECTORY_TREE_H

#include <string>
#include <iostream>
#include <Windows.h>
#include <queue>
#include <thread>
#include "Polyhedron.h"
#include <atomic>
#include "GLFW\glfw3.h"
#include <vector>
#include "Camera.h"
#include "Folder.h"
#include "LineManager.h"
#include "PointManager.h"
#include "glm\gtx\rotate_vector.hpp"
#include "glm\gtx\quaternion.hpp"

// Listing files in a directory (on windows): http://msdn.microsoft.com/en-us/library/aa365200.aspx
class DirectoryTree{
private:
	Folder* root;
	std::string rootPath;
	__int64 totalFolders;
	__int64 totalFiles;
	__int64 totalFileSize;

	// For testing, remove in final /////////////////////
	float TEST_ANGLE;
	
	// For rendering
	glm::vec3* points;
	LineManager		lineMan;
	PointManager	pointMan;
	
	GLuint			shaderProgram;
	GLuint			vao;			// Vertex array object
	GLuint			pointsBuffer;	// Buffer for points

	std::thread buildThread;
	std::atomic<bool> treeIsBuilding;
	std::atomic<bool> treeIsBuilt;
	std::atomic<bool> killThread;
	bool meshIsConstructed;

	void BuildTreeThread();
	void BuildGeometry();
	void DestroyTree(Folder* root);
	int GetDepth(Folder* folder);

public:
	DirectoryTree();
	DirectoryTree(std::string rootPath);
	~DirectoryTree();

	void SetPath(std::string path);
	Folder* GetRoot();
	bool Done();
	void BuildTree();
	void ResetTree();
	void Render(Camera& cam);
	void Update();
};

#endif // _DIRECTORY_TREE_H