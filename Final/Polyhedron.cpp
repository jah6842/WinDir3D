#include "Polyhedron.h"

unsigned int Polyhedron::totalPoints = 0;

#pragma region Constructors
Polyhedron::Polyhedron(glm::vec3 anOrigin, GLfloat rad, GLuint sides){
	origin = anOrigin;
	velocity = glm::vec3(0.0f, 0.0f, 0.0f); // Random velocity in range [1, 5].
	rotVelocity = glm::vec3(0.0f, 0.0f, 0.0f); 
	rotation = glm::quat(glm::vec3(0,0,0)); 
	numFaces = sides;
	radius = rad;
	wireframe = false;
	switch(sides){
		case 4: numPoints = 6; geometry = GL_TRIANGLE_STRIP; break;
		case 6: numPoints = 36; geometry = GL_TRIANGLES; break;
		case 8: numPoints = 12; geometry = GL_TRIANGLE_STRIP; break;
		default: numPoints = 6; sides = 4; geometry = GL_TRIANGLE_STRIP; break;
	}
	color = glm::vec4(0,0,1,1);
	GeneratePoints();
	Init();
}

Polyhedron::Polyhedron(const Polyhedron& otherPolyhedron){
	origin =	otherPolyhedron.origin;
	radius =	otherPolyhedron.radius;
	velocity =	otherPolyhedron.velocity;
	rotVelocity=otherPolyhedron.rotVelocity;
	rotation =	otherPolyhedron.rotation;
	numFaces =	otherPolyhedron.numFaces;
	geometry =	otherPolyhedron.geometry;
	color =		otherPolyhedron.color;
	wireframe = otherPolyhedron.wireframe;
	GeneratePoints();
	Init();
}

Polyhedron& Polyhedron::operator=(const Polyhedron& otherPolyhedron){
	if(this != &otherPolyhedron){
		origin =	otherPolyhedron.origin; 
		radius =	otherPolyhedron.radius;
		velocity =	otherPolyhedron.velocity;
		rotVelocity=otherPolyhedron.rotVelocity;
		rotation =	otherPolyhedron.rotation;
		numFaces =	otherPolyhedron.numFaces;
		geometry =	otherPolyhedron.geometry;
		color =		otherPolyhedron.color;
		wireframe = otherPolyhedron.wireframe;
		GeneratePoints();
		Init();
	}

	return *this;
}

#pragma endregion

// Adds veloicity
void Polyhedron::AddForce(glm::vec3 aForce)
{
	velocity += aForce;
}

// Reverse the velocities ("bounce")
void Polyhedron::ReverseForce(void){
	velocity *= -1;
}

// Adds rotational force
void Polyhedron::AddRotForce(glm::vec3 rForce){
	rotVelocity += rForce;
}

// Initialization
void Polyhedron::Init()
{
	// Create a vertex array object
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

	// Create and initialize a buffer object for each Polyhedron.
    glGenBuffers( 1, &myBuffer );
    glBindBuffer( GL_ARRAY_BUFFER, myBuffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof(glm::vec3) * numPoints, points, GL_STATIC_DRAW );

	// Load shaders and use the resulting shader program
	myShaderProgram = CreateShaderProgram( "vshader.glsl", "fshader.glsl" );

	// Initialize the vertex position attribute from the vertex shader
    //GLuint loc = glGetAttribLocation( myShaderProgram, "vPosition" );
    //glEnableVertexAttribArray( loc );
    //glVertexAttribPointer( loc, 3, GL_FLOAT, GL_FALSE, 0, 0 );

	totalPoints += this->numPoints;
}

// Updates positions and rotations
void Polyhedron::Update(){
	// Move depending on velocity
	origin += velocity * Time::DeltaTime();
	// rotate depending on rotational velocity
	rotation = glm::quat(rotVelocity * Time::DeltaTime()) * rotation;
}

// Renders the object
void Polyhedron::Render(Camera& cam)
{
	// Use the buffer and shader for each Polyhedron.
	glUseProgram( myShaderProgram );

	glBindVertexArray( vao );

	glBindBuffer( GL_ARRAY_BUFFER, myBuffer );

	// Initialize the vertex position attribute from the vertex shader.
    GLuint loc = glGetAttribLocation( myShaderProgram, "vPosition" );
    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 3, GL_FLOAT, GL_FALSE, 0, 0 );

	// model is world coordinates
	glm::mat4 Model = glm::translate(glm::mat4(1.0f), origin) * glm::toMat4(rotation); 
	
	// Combine them all to send to the shader
	glm::mat4 MVP = cam.GetProjection() * cam.GetView() * Model;

	GLint mvpLoc = glGetUniformLocation(myShaderProgram, "MVP");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(MVP));

	GLint colorPos = glGetUniformLocation(myShaderProgram, "inColor");

	//Draw filled
	if(!wireframe){
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glLineWidth(2.0);

		glProgramUniform4fv(myShaderProgram, colorPos, 1, glm::value_ptr(color) );
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(geometry, 0, numPoints);
	} else {
		// Don't cull faces so we can see the whole wireframe
		glDisable(GL_CULL_FACE);
	}
	
	//Draw wireframe
	glEnable( GL_POLYGON_OFFSET_FILL );      
    glPolygonOffset( 1.0f, 1.0f );
	glProgramUniform4fv(myShaderProgram, colorPos, 1, glm::value_ptr(glm::vec4(0.0f,0.0f,0.0f,1.0f)) );
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(geometry, 0, numPoints);
}

// Generates points based on number of sides
void Polyhedron::GeneratePoints(void)
{
	if(points)
		delete[] points;
	
	// Tetrahedron triangle strip
	if(numFaces == 4){
		geometry = GL_TRIANGLE_STRIP;
		points = new glm::vec3[6];
		points[0] = glm::vec3(0.0, 0.0, 1.0) * radius;
		points[1] = glm::vec3(.943f, 0.0, -.333f) * radius;
		points[2] = glm::vec3(-.471f, .816f, -.333f) * radius;
		points[3] = glm::vec3(-.471f, -.816f, -.333f) * radius;
		points[4] = glm::vec3(0.0, 0.0, 1.0) * radius;
		points[5] = glm::vec3(.943f, 0.0, -.333f) * radius;
		return;
	}
	// Cube of built from triangles
	if(numFaces == 6){
		geometry = GL_TRIANGLES;
		points = new glm::vec3[36];
		GLfloat size = radius / 2.0f;
		points[0] = glm::vec3(-size,-size,-size);
		points[1] = glm::vec3(-size,-size, size);
		points[2] = glm::vec3(-size, size, size);
		points[3] = glm::vec3(size, size,-size);
		points[4] = glm::vec3(-size,-size,-size);
		points[5] = glm::vec3(-size, size,-size);
		points[6] = glm::vec3(size,-size, size);
		points[7] = glm::vec3(-size,-size,-size);
		points[8] = glm::vec3(size,-size,-size);
		points[9] = glm::vec3(size, size,-size);
		points[10] = glm::vec3(size,-size,-size);
		points[11] = glm::vec3(-size,-size,-size);
		points[12] = glm::vec3(-size,-size,-size);
		points[13] = glm::vec3(-size, size, size);
		points[14] = glm::vec3(-size, size,-size);
		points[15] = glm::vec3(size,-size, size);
		points[16] = glm::vec3(-size,-size, size);
		points[17] = glm::vec3(-size,-size,-size);
		points[18] = glm::vec3(-size, size, size);
		points[19] = glm::vec3(-size,-size, size);
		points[20] = glm::vec3(size,-size, size);
		points[21] = glm::vec3(size, size, size);
		points[22] = glm::vec3(size,-size,-size);
		points[23] = glm::vec3(size, size,-size);
		points[24] = glm::vec3(size,-size,-size);
		points[25] = glm::vec3(size, size, size);
		points[26] = glm::vec3(size,-size, size);
		points[27] = glm::vec3(size, size, size);
		points[28] = glm::vec3(size, size,-size);
		points[29] = glm::vec3(-size, size,-size);
		points[30] = glm::vec3(size, size, size);
		points[31] = glm::vec3(-size, size,-size);
		points[32] = glm::vec3(-size, size, size);
		points[33] = glm::vec3(size, size, size);
		points[34] = glm::vec3(-size, size, size);
		points[35] = glm::vec3(size,-size, size);
		return;
	}
	// Tetrahedron triangle strip
	if(numFaces == 8){
		geometry = GL_TRIANGLE_STRIP;
		points = new glm::vec3[12];
		points[0] = glm::vec3(0.0, 0.0, 1.0) * radius;
		points[1] = glm::vec3(.943f, 0.0, -.333f) * radius;
		points[2] = glm::vec3(-.471f, .816f, -.333f) * radius;
		points[3] = glm::vec3(-.471f, -.816f, -.333f) * radius;
		points[4] = glm::vec3(0.0, 0.0, 1.0) * radius;
		points[5] = glm::vec3(.943f, 0.0, -.333f) * radius;

		points[6] = glm::vec3(0.0, 0.0, -1.0) * radius;
		points[7] = glm::vec3(-.943f, 0.0, .333f) * radius;
		points[8] = glm::vec3(.471f, -.816f, .333f) * radius;
		points[9] = glm::vec3(.471f, .816f, .333f) * radius;
		points[10] = glm::vec3(0.0, 0.0, -1.0) * radius;
		points[11] = glm::vec3(-.943f, 0.0, .333f) * radius;
		return;
	}
}

// Rotate with euler angles (implemented as glm::quaternion)
void Polyhedron::Rotate(GLfloat x, GLfloat y, GLfloat z){
	rotation = glm::quat(glm::vec3(x,y,z)) * rotation;
}

void Polyhedron::Rotate(glm::vec3 rot){
	rotation = glm::quat(rot) * rotation;
}

// Returns origin as glm::vec3
glm::vec3 Polyhedron::GetOrigin(void)
{
	return origin;
}

// Returns rotation as glm::quaternion
glm::quat Polyhedron::GetRotation(void){
	return rotation;
}

// Returns radius
GLfloat Polyhedron::GetRadius(void)
{
	return radius;
}

// Destructor
Polyhedron::~Polyhedron(void)
{
	if(points != nullptr)
		delete[] points;
}

// Get min and max points for AABB collision detection
void Polyhedron::GetMinMax(glm::vec3& min, glm::vec3& max){
	glm::vec3 curMin = glm::vec3(999,999,999);
	glm::vec3 curMax = glm::vec3(-999,-999,-999);

	for(GLuint i = 0; i < numPoints; i++){
		if(curMin.x > points[i].x + origin.x)
			curMin.x = points[i].x + origin.x;
		if(curMin.y > points[i].y + origin.y)
			curMin.y = points[i].y + origin.y;
		if(curMin.z > points[i].z + origin.z)
			curMin.z = points[i].z + origin.z;
		if(curMax.x < points[i].x + origin.x)
			curMax.x = points[i].x + origin.x;
		if(curMax.y < points[i].y + origin.y)
			curMax.y = points[i].y + origin.y;
		if(curMax.z < points[i].z + origin.z)
			curMax.z = points[i].z + origin.z;
	}

	min = curMin;
	max = curMax;
}

// Sets color to be passed to shader
void Polyhedron::SetColor(glm::vec4 newColor){
	color = newColor;
}

// Draw wireframe only?
void Polyhedron::SetWireframe(GLboolean isWireframe){
	wireframe = isWireframe;
}
