#include "FolderViewer.h"

FolderViewer::FolderViewer(){
	cam = nullptr;
	targetFolder = nullptr;
	freeLook = true;
}

FolderViewer::~FolderViewer(){
	// Nothing to destroy
}

void FolderViewer::Update(){
	float speed = 10000.0f;

	
	lastchanged = glfwGetTime() - lastTime;

	if(Keys::F){
		freeLook = !freeLook;
	}

	if (Keys::A)
		cam->Move(speed * Time::DeltaTime(),0,0);
	if (Keys::D)
		cam->Move(-speed * Time::DeltaTime(),0,0);
	if (Keys::S)
		cam->Move(0,0,-speed * Time::DeltaTime());
	if (Keys::W)
		cam->Move(0,0,speed * Time::DeltaTime());

	if(freeLook){
		if (Keys::LEFT)
			cam->Rotate(0,-80 * Time::DeltaTime(),0);
		if (Keys::RIGHT)
			cam->Rotate(0,80 * Time::DeltaTime(),0);

		if(Keys::SPACE)
			cam->Move(0,-speed * Time::DeltaTime(), 0);
		if(Keys::SHIFT)
			cam->Move(0,speed * Time::DeltaTime(), 0);

		if(Keys::Z)
			cam->LookAt(glm::vec3(0.0f, 0.0f, 0.0f));
	} else {

		if(targetFolder != nullptr){
			cam->LookAt(targetFolder->position);
			

			if(Keys::I && lastchanged > .5f)
				if(targetFolder->parent != nullptr){
					targetFolder = targetFolder->parent;
					PrintLookingAt();
					lastchanged = 0.0f;
					lastTime = glfwGetTime();
				}
			if(Keys::K && lastchanged > .5f){
				if(targetFolder->subfolders.size() > 0){
					targetFolder = targetFolder->subfolders.begin()->second;
					PrintLookingAt();
					lastchanged = 0.0f;
					lastTime = glfwGetTime();
				}
			}
		}
		else
			cam->LookAt(glm::vec3(0.0f));
	}
}

void FolderViewer::Render(){
	if(targetFolder != nullptr){
		std::string path = Folder::GetPath(targetFolder);
		std::replace(path.begin(), path.end(), '\\', '/');
		path = "Currently Looking At: " + path;
		printText2D(path.c_str(), 10, 500, 16);
		printText2D(path.c_str(), 10, 520, 16);
	}
}

void FolderViewer::SetCamera(Camera* c){
	cam = c;
}

void FolderViewer::PrintLookingAt(){
	if(targetFolder != nullptr)
		std::cout << "Currently Looking At: " << Folder::GetPath(targetFolder) << std::endl;
}

void FolderViewer::SetTarget(Folder* f){
	targetFolder = f;
	PrintLookingAt();
}

bool FolderViewer::HasTarget(){
	if(targetFolder != nullptr)
		return true;
	else
		return false;
}

void FolderViewer::FreeLook(bool canFreelook){
	freeLook = canFreelook;
}
