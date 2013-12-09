#include "Main.h"

#include "Text2D.h"

GLFWwindow* window;
DirectoryTree* dirTree;
Camera cam(800,600,60.0f);
FolderViewer fViewer;

void update(){
	// Update global delta time
	Time::Update();

	// Update the folder viewer that controls the camera
	fViewer.Update();

	// Update directory tree
	if(dirTree != nullptr){
		dirTree->Update();

		if(dirTree->Done() && !fViewer.HasTarget())
			fViewer.SetTarget(dirTree->GetRoot());
	}
}

void display(){
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );     // clear the window

	dirTree->Render(cam);

	fViewer.Render();
}

void errorCallback(int error, const char* description){
    //fputs(description, stderr);
	std::cout << description << std::endl;
}

void windowCloseCallback(GLFWwindow* window){
	// Cleanup here if necessary
	std::cout << "Closing window."  << std::endl;
}

void windowResize(GLFWwindow* window, int width, int height){
	// recalc VP
	glViewport(0, 0, width, height);
	cam.Resize(width, height);
	update();
	display();
	glfwSwapBuffers(window);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	Keys::Input(key, scancode, action, mods);
}

// Mouse wheel scrolled
// xOffset never changes from 0?
// yOffset is -1 if scrolled down (towards self), 1 if scrolled up (away from self)
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset){

}

// Initialize everything we need
void init(){
	// Seed random number generator with time
	srand(static_cast<unsigned int>(time(NULL)));

	// Initialize GLFW
	if(!glfwInit())
		exit(EXIT_FAILURE);

	// Gets a struct with monitor statistics
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	
	//window = glfwCreateWindow(mode->width,mode->height,"",glfwGetPrimaryMonitor(),NULL);
	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	window = glfwCreateWindow(800,600,"first",NULL,NULL);
	if (!window){
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

	// Make the window's context current
    glfwMakeContextCurrent(window);

	// Set callbacks
	glfwSetErrorCallback(errorCallback);
	glfwSetWindowCloseCallback(window, windowCloseCallback);
	glfwSetWindowSizeCallback(window, windowResize);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetScrollCallback(window, scrollCallback);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
    glewInit();
	
	// 0 = no vsync, 1 = vsync
	glfwSwapInterval(1);
	
	// OpenGL
	glClearColor( 1.0, 1.0, 1.0, 1.0 ); 

	// Set the camera controlled by the file viewer
	fViewer.SetCamera(&cam);

	// Initialize text rendering
	initText2D("font.bmp");
}

// Clean up our own memory structures
void cleanup(){
	delete dirTree;
	cleanupText2D();
}

int main(int argc, char* argv[]){

	// Initialize everything
	init();

	dirTree = new DirectoryTree("H:\\New Folder");
	dirTree->BuildTree();

	// Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
		// Poll for and process events
        glfwPollEvents();

		// updates
		update();

        // Render here
		display();

        // Swap front and back buffers
        glfwSwapBuffers(window);
    }

	// Terminates GLFW windows and cleans up their memory
    glfwTerminate();
	// Clean up our own memory structures
	cleanup();
    return 0;
}