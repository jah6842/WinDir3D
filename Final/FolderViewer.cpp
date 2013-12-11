#include "FolderViewer.h"

FolderViewer::FolderViewer(){
	cam = nullptr;
	targetFolder = nullptr;
	freeLook = true;
	int curFolder = 0;
}

FolderViewer::~FolderViewer(){
	// Nothing to destroy
}

void FolderViewer::Update(){
	float speed = 10000.0f;
	
	lastchanged = glfwGetTime() - lastTime;

	if(Keys::F && lastchanged > 1.0f){
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

	if(Keys::SPACE)
			cam->Move(0,-speed * Time::DeltaTime(), 0);
	if(Keys::SHIFT)
		cam->Move(0,speed * Time::DeltaTime(), 0);

	if(freeLook){
		if (Keys::LEFT)
			cam->Rotate(0,-80 * Time::DeltaTime(),0);
		if (Keys::RIGHT)
			cam->Rotate(0,80 * Time::DeltaTime(),0);

		if(Keys::Z)
			cam->LookAt(glm::vec3(0.0f, 0.0f, 0.0f));
	} else {

		if(targetFolder != nullptr){
			cam->LookAt(targetFolder->position);
			
			if(Keys::I && lastchanged > .25f)
				if(targetFolder->parent != nullptr){
					targetFolder = targetFolder->parent;
					PrintLookingAt();
					lastchanged = 0.0f;
					lastTime = glfwGetTime();
					curFolder = 0;
				}
			if(Keys::K && lastchanged > .25f){
				if(targetFolder->subfolders.size() > 0){
					targetFolder = targetFolder->subfolders.begin()->second;
					PrintLookingAt();
					lastchanged = 0.0f;
					lastTime = glfwGetTime();
					curFolder = 0;
				}
			}

			if(Keys::L && lastchanged > .25f){
				Folder* temp = targetFolder->GetNextFolder(curFolder);
				if(temp != nullptr){
					targetFolder = temp;
					curFolder++;
					lastchanged = 0.0f;
					lastTime = glfwGetTime();
				}
			}

			if(Keys::J && lastchanged > .25f){
				Folder* temp = targetFolder->GetPreviousFolder(curFolder);
				if(temp != nullptr){
					targetFolder = temp;
					curFolder--;
					if(curFolder < 0)
						curFolder = 0;
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
		printText2D("Press ESC to exit at any time (do not close console window manually).", 0, 32, 16);
		printText2D("Use F to lock onto the targeted folder and IJKL to move through the folder tree.", 0, 32, 16);
		printText2D("Use WASD/Shift/Space to move around. Hold Z to lock onto the root folder.", 0, 16, 16);
		printText2D(path.c_str(), 0, 0, 16);
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
