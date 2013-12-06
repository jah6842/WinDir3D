#ifndef _POINT_MANAGER_H
#define _POINT_MANAGER_H

#include <vector>
#include "Folder.h"
#include "Camera.h"
#include "GL\glew.h"
#include "Time.h"
#include "Keys.h"
#include "MyLoader.h"

class PointManager { 
private:

	std::vector<File*> files;
	int totalPoints;
	int pointCount;
	float pointSize;

	glm::vec3 * points;

	GLuint			myShaderProgram;// Shader program to use
	glm::vec4		color;			// R,G,B,A
	GLuint			vao;			// Vertex array object
	GLuint			myBuffer;		// Buffer for points

	void Init();

public:

	PointManager();
	~PointManager();

	void AddPoint(File* a);
	void GenerateGeometry();
	void Render(Camera& cam);
};

#endif // _POINT_MANAGER_H