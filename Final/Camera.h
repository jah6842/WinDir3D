#ifndef _CAMERA_H
#define _CAMERA_H

#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <glm\gtx\quaternion.hpp>
#include <glm\gtc\quaternion.hpp>
#include <iostream>

class Camera {
private:
	// Camera properties
	glm::mat4 _Projection;					// Projection Matrix
	glm::mat4 _View;							// View Matrix
	glm::vec3 _position;						// x,y,z
	//glm::quat _rotation;						// quaternion rotation
	float _nearClipDist, _farClipDist;	// Clipping distances
	float _FoV;							// Field of view
	int _width, _height;				// Pixel width/height of viewport

	void RecalcProjMatrix();

public:

	Camera(int w, int h, float fov);
	Camera(const Camera& other);
	Camera& operator=(const Camera& other);
	~Camera();

	void Move(float x, float y, float z);
	void Rotate(float x, float y, float z);
	
	glm::vec3 GetPosition(void);
	glm::mat4 GetProjection(void);
	glm::mat4 GetView(void);

	void LookAt(glm::vec3 targetPos);

	void Resize(int w, int h);
};

#endif //_CAMERA_H