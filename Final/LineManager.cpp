#include "LineManager.h"

LineManager::LineManager(){
	color = glm::vec4(0.0f, 0.0f, 0.5f, .4f);
	totalLines = 0;
	shown = 0;
	lastShown = 0.0f;
	points = nullptr;
	Init();
}

LineManager::~LineManager(){
	if(points != nullptr)
		delete[] points;
}

void LineManager::Init(){
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

void LineManager::GenerateGeometry(){

	if(points != nullptr)
		delete[] points;

	points = new glm::vec3[lines.size() * 2];

	int count = 0;
	for(int i = 0; i < lines.size(); i++){
		points[count++] = lines[i].first->position;
		points[count++] = lines[i].second->position;
	}

	totalLines = count / 2;

	glBindVertexArray( vao );
	glBindBuffer( GL_ARRAY_BUFFER, myBuffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof(glm::vec3) * totalLines * 2, points, GL_STATIC_DRAW );
}

void LineManager::AddPair(Folder* a, Folder* b){
	lines.push_back( std::make_pair(a,b) );
}

void LineManager::Render(Camera& cam){
	
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

	if(Keys::R)
		shown = 0;
	if(shown < totalLines * 2)
		shown += 1000;

	//Draw
	glLineWidth(.5f);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glProgramUniform4fv(myShaderProgram, colorPos, 1, glm::value_ptr(color) );
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_LINES, 0, shown);
}