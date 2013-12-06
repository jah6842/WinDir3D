#ifndef _LINE_MANAGER_H
#define _LINE_MANAGER_H

#include <vector>
#include "Folder.h"
#include "Camera.h"
#include "GL\glew.h"
#include "Time.h"
#include "Keys.h"
#include "MyLoader.h"

class LineManager { 
private:

	std::vector<std::pair<Folder*,Folder*>> lines;
	int totalLines;
	double lastShown;
	int shown;

	glm::vec3 * points;

	GLuint			myShaderProgram;// Shader program to use
	glm::vec4		color;			// R,G,B,A
	GLuint			vao;			// Vertex array object
	GLuint			myBuffer;		// Buffer for points

	void Init();

public:

	LineManager();
	~LineManager();

	void AddPair(Folder* a, Folder* b);
	void GenerateGeometry();
	void Render(Camera& cam);
};

#endif // _LINE_MANAGER_H