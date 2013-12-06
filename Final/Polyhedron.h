#ifndef _POLYHEDRON_H
#define _POLYHEDRON_H

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\constants.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "MyLoader.h"
#include "Camera.h"
#include "Time.h"

#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062

class Polyhedron
{
	public:
		static unsigned int totalPoints;

		Polyhedron(glm::vec3 anOrigin, GLfloat rad, GLuint sides);
		Polyhedron(const Polyhedron& otherPolyhedron);
		Polyhedron& Polyhedron::operator=(const Polyhedron& otherPolyhedron);
		virtual ~Polyhedron(void);

		// Initialization
		void Init(void);
		// Renders the object
		void Render(Camera& cam);
		// Updates positions and rotation
		void Update(void);
		// Add velocity
		void AddForce(glm::vec3 aForce);
		// Reverse the velocities ("bounce")
		void ReverseForce(void);
		// Adds rotational force
		void AddRotForce(glm::vec3 rForce);
		// Returns origin as glm::vec3
		glm::vec3 GetOrigin(void);
		// Returns rotation as glm::quaternion
		glm::quat GetRotation(void);
		// Returns radius
		GLfloat GetRadius(void);
		// Rotate with euler angles (implemented as glm::quaternion)
		void Rotate(GLfloat x, GLfloat y, GLfloat z);
		void Rotate(glm::vec3 rot);
		// Get min and max points for AABB collision detection
		void GetMinMax(glm::vec3& min, glm::vec3& max);
		// Sets color to be passed to shader
		void SetColor(glm::vec4 newColor);
		// Draw wireframe only?
		void SetWireframe(GLboolean isWireframe);

	private:
		void GeneratePoints(void);
		
		glm::vec4			color;		// R,G,B,A
		glm::vec3			origin;		// x,y,z
		glm::quat			rotation;	// glm::quaternion
		glm::vec3			velocity;	// x,y,z
		glm::vec3			rotVelocity;// Rotational velocity
		glm::vec3*			points;		// Array of points for the model
		GLuint			numPoints;	// Number of points in points
		GLuint			numFaces;	// Number of faces on the model
		GLuint			geometry;	// Type of drawing to use (GL_TRIANGLES, GL_TRIANGLE_STRIP, etc)
		GLfloat			radius;		// Radius of the object

		GLboolean		wireframe;	// Draw wireframe only?

		GLuint	myShaderProgram;
		GLuint			vao;		// Vertex array object
		GLuint			myBuffer;	// Buffer for points
};

#endif // _POLYHEDRON_H