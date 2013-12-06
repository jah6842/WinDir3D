#ifndef _TIME_H
#define _TIME_H

#include <GLFW\glfw3.h>

class Time {
private:
	static double currentFrame;
	static double lastFrame;
	static double deltaTime;
public:
	static void Update(){
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}
	static float DeltaTime(){
		return static_cast<float>(deltaTime);
	}
};

#endif // _TIME_H