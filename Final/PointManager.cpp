#include "PointManager.h"

PointManager::PointManager(){
	color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	totalPoints = 0;
	pointCount = 0;
	pointSize = .5f;
	points = nullptr;
	Init();
}

PointManager::~PointManager(){
	if(points != nullptr)
		delete[] points;
}

void PointManager::Init(){
	// Create a vertex array object
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

	// Create and initialize a buffer object.
    glGenBuffers( 1, &myBuffer );
    
	// Load shaders and use the resulting shader program
	myShaderProgram = CreateShaderProgram( "vshader.glsl", "fshader.glsl" );

	// Initialize the vertex position attribute from the vertex shader
    GLuint loc = glGetAttribLocation( myShaderProgram, "vPosition" );
    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 3, GL_FLOAT, GL_FALSE, 0, 0 );
}

void PointManager::GenerateGeometry(){

	if(points != nullptr)
		delete[] points;

	points = new glm::vec3[files.size() * 36];

	for(int i = 0; i < files.size(); i++){
		float radiusOfCube = .2f;
		points[pointCount++] =( files[i]->position + glm::vec3(-radiusOfCube,-radiusOfCube,-radiusOfCube));
		points[pointCount++] =( files[i]->position + glm::vec3(-radiusOfCube,-radiusOfCube, radiusOfCube));
		points[pointCount++] =( files[i]->position + glm::vec3(-radiusOfCube, radiusOfCube, radiusOfCube));
		points[pointCount++] =( files[i]->position + glm::vec3(radiusOfCube, radiusOfCube,-radiusOfCube));
		points[pointCount++] =( files[i]->position + glm::vec3(-radiusOfCube,-radiusOfCube,-radiusOfCube));
		points[pointCount++] =( files[i]->position + glm::vec3(-radiusOfCube, radiusOfCube,-radiusOfCube));
		points[pointCount++] =( files[i]->position + glm::vec3(radiusOfCube,-radiusOfCube, radiusOfCube));
		points[pointCount++] =( files[i]->position + glm::vec3(-radiusOfCube,-radiusOfCube,-radiusOfCube));
		points[pointCount++] =( files[i]->position + glm::vec3(radiusOfCube,-radiusOfCube,-radiusOfCube));
		points[pointCount++] =( files[i]->position + glm::vec3(radiusOfCube, radiusOfCube,-radiusOfCube));
		points[pointCount++] =( files[i]->position + glm::vec3(radiusOfCube,-radiusOfCube,-radiusOfCube));
		points[pointCount++] =( files[i]->position + glm::vec3(-radiusOfCube,-radiusOfCube,-radiusOfCube));
		points[pointCount++] =( files[i]->position + glm::vec3(-radiusOfCube,-radiusOfCube,-radiusOfCube));
		points[pointCount++] =( files[i]->position + glm::vec3(-radiusOfCube, radiusOfCube, radiusOfCube));
		points[pointCount++] =( files[i]->position + glm::vec3(-radiusOfCube, radiusOfCube,-radiusOfCube));
		points[pointCount++] =( files[i]->position + glm::vec3(radiusOfCube,-radiusOfCube, radiusOfCube));
		points[pointCount++] =( files[i]->position + glm::vec3(-radiusOfCube,-radiusOfCube, radiusOfCube));
		points[pointCount++] =( files[i]->position + glm::vec3(-radiusOfCube,-radiusOfCube,-radiusOfCube));
		points[pointCount++] =( files[i]->position + glm::vec3(-radiusOfCube, radiusOfCube, radiusOfCube));
		points[pointCount++] =( files[i]->position + glm::vec3(-radiusOfCube,-radiusOfCube, radiusOfCube));
		points[pointCount++] =( files[i]->position + glm::vec3(radiusOfCube,-radiusOfCube, radiusOfCube));
		points[pointCount++] =( files[i]->position + glm::vec3(radiusOfCube, radiusOfCube, radiusOfCube));
		points[pointCount++] =( files[i]->position + glm::vec3(radiusOfCube,-radiusOfCube,-radiusOfCube));
		points[pointCount++] =( files[i]->position + glm::vec3(radiusOfCube, radiusOfCube,-radiusOfCube));
		points[pointCount++] =( files[i]->position + glm::vec3(radiusOfCube,-radiusOfCube,-radiusOfCube));
		points[pointCount++] =( files[i]->position + glm::vec3(radiusOfCube, radiusOfCube, radiusOfCube));
		points[pointCount++] =( files[i]->position + glm::vec3(radiusOfCube,-radiusOfCube, radiusOfCube));
		points[pointCount++] =( files[i]->position + glm::vec3(radiusOfCube, radiusOfCube, radiusOfCube));
		points[pointCount++] =( files[i]->position + glm::vec3(radiusOfCube, radiusOfCube,-radiusOfCube));
		points[pointCount++] =( files[i]->position + glm::vec3(-radiusOfCube, radiusOfCube,-radiusOfCube));
		points[pointCount++] =( files[i]->position + glm::vec3(radiusOfCube, radiusOfCube, radiusOfCube));
		points[pointCount++] =( files[i]->position + glm::vec3(-radiusOfCube, radiusOfCube,-radiusOfCube));
		points[pointCount++] =( files[i]->position + glm::vec3(-radiusOfCube, radiusOfCube, radiusOfCube));
		points[pointCount++] =( files[i]->position + glm::vec3(radiusOfCube, radiusOfCube, radiusOfCube));
		points[pointCount++] =( files[i]->position + glm::vec3(-radiusOfCube, radiusOfCube, radiusOfCube));
		points[pointCount++] =( files[i]->position + glm::vec3(radiusOfCube,-radiusOfCube, radiusOfCube));
	}

	totalPoints = pointCount;

	glBindBuffer( GL_ARRAY_BUFFER, myBuffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof(glm::vec3) * totalPoints * 36, points, GL_STATIC_DRAW );
}

void PointManager::AddPoint(File* a){
	files.push_back( a );
}

void PointManager::Render(Camera& cam){
	
	if(Keys::Y)
		pointSize++;

	int err = glGetError();
	// Use the buffer and shader for each Polyhedron.
	glUseProgram( myShaderProgram );
	err = glGetError();
	glBindVertexArray( vao );
	glBindBuffer( GL_ARRAY_BUFFER, myBuffer );

	// Initialize the vertex position attribute from the vertex shader.
    GLuint loc = glGetAttribLocation( myShaderProgram, "vPosition" );
    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 3, GL_FLOAT, GL_FALSE, 0, 0 );

	// model is world coordinates
	glm::mat4 Model = glm::mat4(1.0f); 
	
	// Combine them all to send to the shader
	glm::mat4 MVP = cam.GetProjection() * cam.GetView() * Model;

	GLint mvpLoc = glGetUniformLocation(myShaderProgram, "MVP");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(MVP));

	GLint colorPos = glGetUniformLocation(myShaderProgram, "inColor");

	//Draw
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glProgramUniform4fv(myShaderProgram, colorPos, 1, glm::value_ptr(color) );
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, totalPoints);
}