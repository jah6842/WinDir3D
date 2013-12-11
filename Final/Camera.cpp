#include "Camera.h"

Camera::Camera(int w, int h, float fov){
	_FoV = fov;
	_width = w;
	_height = h;
	_nearClipDist = .1f;
	_farClipDist = 10000000.0f;
	_position = glm::vec3(0.0f,0.0f,-200.0f);
	//_rotation = glm::quat(glm::vec3(0.0f,0.0f,0.0f));
	RecalcProjMatrix();
}

Camera::Camera(const Camera& other){
	_FoV =			other._FoV;
	_width =		other._width;
	_height =		other._height;
	_nearClipDist = other._nearClipDist;
	_farClipDist =	other._farClipDist;
	_position =		other._position;
	//_rotation =		other._rotation;
	_Projection =	other._Projection;
	_View =			other._View;
}

Camera& Camera::operator=(const Camera& other){
	if(this != &other){
		_FoV =			other._FoV;
		_width =		other._width;
		_height =		other._height;
		_nearClipDist = other._nearClipDist;
		_farClipDist =	other._farClipDist;
		_position =		other._position;
		//_rotation =		other._rotation;
		_Projection =	other._Projection;
		_View =			other._View;
	}
	return *this;
}

Camera::~Camera(){
	// Nothing to destroy
}

void Camera::RecalcProjMatrix(){
	_Projection = glm::perspective(_FoV,							// Field of View
			static_cast<float>(_width)/static_cast<float>(_height),	// Aspect ratio (width/height)
			_nearClipDist,											// Near plane
			_farClipDist);											// Far plane
}

// http://classes.soe.ucsc.edu/cmps160/Spring13/projects/gaperez/proj/source/160Project/Camera.cpp
void Camera::Move(float x, float y, float z){
	glm::mat4 translation(1.0f);
	//want to zoom in by moving everything negative z
	//offset = -offset;
	glm::vec3 offset(x,y,z);
	translation = glm::translate(translation, offset);
	_View = translation * _View;
	//get object coorinates of camera.
	_position = glm::vec3(glm::inverse(_View) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
}

void Camera::Rotate(float x, float y, float z){
	glm::mat4 allAxes =  glm::mat4(1.0f);
	if(x != 0.0f) 
		allAxes = glm::rotate(allAxes, x, glm::vec3(1.0f, 0.0f, 0.0f));
	if(y != 0.0f)
		allAxes = glm::rotate(allAxes, y, glm::vec3(0.0f, 1.0f, 0.0f));
	if(z != 0.0f)
		allAxes = glm::rotate(allAxes, z, glm::vec3(0.0f, 0.0f, 1.0f));

	_View = allAxes * _View;
	//get object coorinates of camera.
	_position = glm::vec3(glm::inverse(_View) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
}

glm::vec3 Camera::GetPosition(){
	return _position;
}

glm::mat4 Camera::GetProjection(){
	return _Projection;
}

glm::mat4 Camera::GetView(){
	return _View;
}

void Camera::LookAt(glm::vec3 targetPos){
	_View = glm::lookAt(_position,
		targetPos,
		glm::vec3(0.0f,1.0f,0.0f));
}

void Camera::Resize(int w, int h){
	_width = w;
	_height = h;
	RecalcProjMatrix();
}