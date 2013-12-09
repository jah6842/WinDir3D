#include "DirectoryTree.h"

void DirectoryTree::SetPath(std::string path){
	rootPath = path;
}

void DirectoryTree::BuildTree(){
	ResetTree();
	buildThread = std::thread(&DirectoryTree::BuildTreeThread, this);
}

void DirectoryTree::BuildTreeThread(){
	treeIsBuilding = true;
	root = new Folder(nullptr, rootPath);
	std::queue<Folder*> queuedFolders;
	queuedFolders.push(root);
	std::cout << "Building hierarchy for: " << Folder::GetPath(root) << std::endl;
	std::cout << "Change the root path in the main() function (for now)!" << std::endl;

	WIN32_FIND_DATA fileOrFolderData;
	LARGE_INTEGER filesize;
	std::string bufferPath;
	HANDLE fileHandle = INVALID_HANDLE_VALUE;
	DWORD dwError=0;

	double start = glfwGetTime();
	double lastTime = glfwGetTime();
	double timeCheck = 0.0f;

	while(queuedFolders.size() > 0) {
		// Prepare string for use with FindFile functions.  First, copy the
		// string to a buffer, then append '*' to the directory name.
		bufferPath = Folder::GetPath(queuedFolders.front());
		bufferPath.append("*");

		if(glfwInit()){
			timeCheck += glfwGetTime() - lastTime;
			lastTime = glfwGetTime();
			if(timeCheck > 1.0f){
				std::cout << "Folders: " << totalFolders << ", Files: " << totalFiles << std::endl;
				timeCheck = 0.0f;
			}
		}

		// Find the first file in the directory.
		fileHandle = FindFirstFile(bufferPath.c_str(), &fileOrFolderData);
		// If the handle is invalid, pop from queue and go to the next folder.
		if (INVALID_HANDLE_VALUE == fileHandle) {
			//std::cout << bufferPath << " invalid." << std::endl;
			queuedFolders.pop();
			continue;
		} 

		// List all the files in the directory with some info about them.
		do {
			// If the folder is . or .., skip it
			if(	 fileOrFolderData.cFileName[0] == '.' || 
				(fileOrFolderData.cFileName[0] == '.' && fileOrFolderData.cFileName[1] == '.'))
					continue;

			// This item is a folder, add it to the queue and insert it into the parent Folder's subfolders map
			if (fileOrFolderData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				Folder* tempF = new Folder(queuedFolders.front(), fileOrFolderData.cFileName);
				queuedFolders.front()->subfolders.insert(std::pair<std::string, Folder*>(fileOrFolderData.cFileName, tempF));
				queuedFolders.push(tempF);
				totalFolders++;
			}

			// This item is a file, insert it into the parent Folder's files map
			else {
				filesize.LowPart = fileOrFolderData.nFileSizeLow;
				filesize.HighPart = fileOrFolderData.nFileSizeHigh;
				//printf(TEXT("%s   %ld bytes\n"), fileOrFolderData.cFileName, filesize.QuadPart);
				File* tempFile = new File(fileOrFolderData.cFileName, filesize.QuadPart);
				//queuedFolders.front()->files.insert(std::pair<std::string, File*>(fileOrFolderData.cFileName, tempFile));
				queuedFolders.front()->AddFile(fileOrFolderData.cFileName, tempFile);
				totalFiles++;
				totalFileSize += filesize.QuadPart;
			}
			
#if __DEBUG
			// Print errors if any occurred.
			dwError = GetLastError();
			switch(dwError){
				case ERROR_SUCCESS: break;
				case ERROR_NO_MORE_FILES: break;
				case ERROR_ACCESS_DENIED: std::cout << "Access denied: " << bufferPath << std::endl; break;
				default: std::cout << "Unknown error:" << dwError << std::endl; break;
			}
#endif // __DEBUG

			// This thread needs to stop, NOW. Destroy should be able to clean up all already allocated resources.
			if(killThread){
				FindClose(fileHandle); // Play nice with file handles c:
				return;
			}

		} while (FindNextFile(fileHandle, &fileOrFolderData) != 0);

		FindClose(fileHandle);

		queuedFolders.pop();
	}

	std::cout << std::endl;
	std::cout << "Total folders: " << totalFolders << std::endl;
	std::cout << "Total files:   " << totalFiles << std::endl;
	std::cout << "Elapsed time:  " << glfwGetTime() - start << " seconds." << std::endl;
	std::cout << "Created file hierarchy." << std::endl;

	treeIsBuilding = false;
	treeIsBuilt = true;
}

void DirectoryTree::BuildGeometry(){
	if(points != nullptr)
		delete[] points;
	points = new glm::vec3[totalFolders * 36];
	int pointCount = 0;

	std::queue<Folder*> queuedFolders;
	queuedFolders.push(root);
	
	Folder* curFolder = root;

	while(queuedFolders.size() > 0){
		curFolder = queuedFolders.front();

		int numSubfolders = curFolder->subfolders.size();

		int count = 0;
		for(std::unordered_map<std::string, Folder*>::iterator itr = curFolder->subfolders.begin(); itr != curFolder->subfolders.end(); itr++){
			
			glm::vec4 offset = itr->second->parent->direction;// 
			GLfloat angle = 0.0f;
			GLfloat divisions;
			glm::quat rot;

			// If this is the first 
			if(itr->second->parent->parent == nullptr){
				GLfloat inc = PI * (3.0f - sqrt(5.0f));
				GLfloat off = 2.0f / numSubfolders;
				GLfloat y = count * off - 1.0f + (off / 2.0f);
				GLfloat r = sqrt(1 - y * y);
				GLfloat phi = count * inc;

				offset = glm::vec4(cos(phi) * r, y, sin(phi) * r, 1.0f);
				angle =  120.0f;
			} 
			else {
				if(numSubfolders == 1){
					divisions = 0;
					if(itr->second->parent->angle >= 180.0f)
						angle = itr->second->parent->angle / 2.0f;
					else
						angle = itr->second->parent->angle;
				}
				else{
					divisions = static_cast<GLfloat>(count) / static_cast<GLfloat>(numSubfolders - 1) ;
					angle = itr->second->parent->angle * .5f;
					GLfloat angle2 = angle * divisions;
					rot = glm::quat(glm::vec3(0.0f, 0.0f , glm::radians(angle)));
					glm::vec4 offset2 = glm::rotate(rot, itr->second->parent->direction);
					offset = glm::rotate(offset2, glm::radians((360.0f * 60.0f) * divisions), glm::vec3(itr->second->parent->direction));
					offset += offset2;
					offset = glm::normalize(offset);
				}
			}

			glm::vec3 pos = itr->second->parent->position;

			float size = itr->second->localSize / 1024.0f / 1024.0f / 1024.0f;
			
			float len = 1000.0f * numSubfolders;
			if(len > 100000.0f)
				len = 100000.0f;
			pos += glm::vec3(offset) * (len + 1000.0f);

			itr->second->position = pos;
			itr->second->radius = .5f + size;
			itr->second->direction = offset;
			itr->second->angle = angle;

			int numFiles = itr->second->files.size();
			int _c = 0;
			
			for(std::unordered_map<std::string, File*>::iterator itr2 = itr->second->files.begin(); itr2 != itr->second->files.end(); itr2++){
				GLfloat inc = PI * (3.0f - sqrt(5.0f));
				GLfloat off = 2.0f / numFiles;
				GLfloat y = _c * off - 1.0f + (off / 2.0f);
				GLfloat r = sqrt(1 - y * y);
				GLfloat phi = _c * inc;

				glm::vec4 temp = glm::vec4(cos(phi) * r, y, sin(phi) * r, 1.0f) ;
				itr2->second->position = pos + (glm::vec3(temp) * 200.0f);// * TEST_ANGLE;
				pointMan.AddPoint(itr2->second);
				_c++;
			}

			lineMan.AddPair(itr->second, itr->second->parent);

			float radiusOfCube = 40.0f + size * (TEST_ANGLE);

			points[pointCount++] =(pos + glm::vec3(-radiusOfCube,-radiusOfCube,-radiusOfCube));
			points[pointCount++] =(pos + glm::vec3(-radiusOfCube,-radiusOfCube, radiusOfCube));
			points[pointCount++] =(pos + glm::vec3(-radiusOfCube, radiusOfCube, radiusOfCube));
			points[pointCount++] =(pos + glm::vec3(radiusOfCube, radiusOfCube,-radiusOfCube));
			points[pointCount++] =(pos + glm::vec3(-radiusOfCube,-radiusOfCube,-radiusOfCube));
			points[pointCount++] =(pos + glm::vec3(-radiusOfCube, radiusOfCube,-radiusOfCube));
			points[pointCount++] =(pos + glm::vec3(radiusOfCube,-radiusOfCube, radiusOfCube));
			points[pointCount++] =(pos + glm::vec3(-radiusOfCube,-radiusOfCube,-radiusOfCube));
			points[pointCount++] =(pos + glm::vec3(radiusOfCube,-radiusOfCube,-radiusOfCube));
			points[pointCount++] =(pos + glm::vec3(radiusOfCube, radiusOfCube,-radiusOfCube));
			points[pointCount++] =(pos + glm::vec3(radiusOfCube,-radiusOfCube,-radiusOfCube));
			points[pointCount++] =(pos + glm::vec3(-radiusOfCube,-radiusOfCube,-radiusOfCube));
			points[pointCount++] =(pos + glm::vec3(-radiusOfCube,-radiusOfCube,-radiusOfCube));
			points[pointCount++] =(pos + glm::vec3(-radiusOfCube, radiusOfCube, radiusOfCube));
			points[pointCount++] =(pos + glm::vec3(-radiusOfCube, radiusOfCube,-radiusOfCube));
			points[pointCount++] =(pos + glm::vec3(radiusOfCube,-radiusOfCube, radiusOfCube));
			points[pointCount++] =(pos + glm::vec3(-radiusOfCube,-radiusOfCube, radiusOfCube));
			points[pointCount++] =(pos + glm::vec3(-radiusOfCube,-radiusOfCube,-radiusOfCube));
			points[pointCount++] =(pos + glm::vec3(-radiusOfCube, radiusOfCube, radiusOfCube));
			points[pointCount++] =(pos + glm::vec3(-radiusOfCube,-radiusOfCube, radiusOfCube));
			points[pointCount++] =(pos + glm::vec3(radiusOfCube,-radiusOfCube, radiusOfCube));
			points[pointCount++] =(pos + glm::vec3(radiusOfCube, radiusOfCube, radiusOfCube));
			points[pointCount++] =(pos + glm::vec3(radiusOfCube,-radiusOfCube,-radiusOfCube));
			points[pointCount++] =(pos + glm::vec3(radiusOfCube, radiusOfCube,-radiusOfCube));
			points[pointCount++] =(pos + glm::vec3(radiusOfCube,-radiusOfCube,-radiusOfCube));
			points[pointCount++] =(pos + glm::vec3(radiusOfCube, radiusOfCube, radiusOfCube));
			points[pointCount++] =(pos + glm::vec3(radiusOfCube,-radiusOfCube, radiusOfCube));
			points[pointCount++] =(pos + glm::vec3(radiusOfCube, radiusOfCube, radiusOfCube));
			points[pointCount++] =(pos + glm::vec3(radiusOfCube, radiusOfCube,-radiusOfCube));
			points[pointCount++] =(pos + glm::vec3(-radiusOfCube, radiusOfCube,-radiusOfCube));
			points[pointCount++] =(pos + glm::vec3(radiusOfCube, radiusOfCube, radiusOfCube));
			points[pointCount++] =(pos + glm::vec3(-radiusOfCube, radiusOfCube,-radiusOfCube));
			points[pointCount++] =(pos + glm::vec3(-radiusOfCube, radiusOfCube, radiusOfCube));
			points[pointCount++] =(pos + glm::vec3(radiusOfCube, radiusOfCube, radiusOfCube));
			points[pointCount++] =(pos + glm::vec3(-radiusOfCube, radiusOfCube, radiusOfCube));
			points[pointCount++] =(pos + glm::vec3(radiusOfCube,-radiusOfCube, radiusOfCube));

			queuedFolders.push(itr->second);
			count++;
		}

		queuedFolders.pop();
	}

	lineMan.GenerateGeometry();
	pointMan.GenerateGeometry();

	/* OpenGL setup */
	// Create a vertex array object
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

	// Create and initialize a buffer object for each Polyhedron.
    glGenBuffers( 1, &pointsBuffer );
    glBindBuffer( GL_ARRAY_BUFFER, pointsBuffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof(glm::vec3) * totalFolders * 36, points, GL_STATIC_DRAW );

	// Load shaders and use the resulting shader program
	shaderProgram = CreateShaderProgram( "vshader.glsl", "fshader.glsl" );

	std::cout << "Total points: " << totalFolders * 36 << std::endl;
}

void DirectoryTree::Render(Camera& cam){
	// Tree isn't built, don't bother rendering.
	if(!Done())
		return;

	glUseProgram( shaderProgram );
	glBindVertexArray( vao );
	glBindBuffer( GL_ARRAY_BUFFER, pointsBuffer );

	// Initialize the vertex position attribute from the vertex shader.
    GLuint loc = glGetAttribLocation( shaderProgram, "vPosition" );
    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 3, GL_FLOAT, GL_FALSE, 0, 0 );

	// model is world coordinates
	glm::mat4 Model = glm::mat4(1.0f); 
	
	// Combine them all to send to the shader
	glm::mat4 MVP = cam.GetProjection() * cam.GetView() * Model;

	GLint mvpLoc = glGetUniformLocation(shaderProgram, "MVP");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(MVP));

	GLint colorPos = glGetUniformLocation(shaderProgram, "inColor");

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glLineWidth(2.0);
	glProgramUniform4fv(shaderProgram, colorPos, 1, glm::value_ptr(glm::vec4(1.0f,0.0f,0.0f,1.0f)) );
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, totalFolders * 36);

	//Draw wireframe
	glEnable( GL_POLYGON_OFFSET_FILL );      
    glPolygonOffset( 1.0f, 1.0f );
	glProgramUniform4fv(shaderProgram, colorPos, 1, glm::value_ptr(glm::vec4(0.0f,0.0f,0.0f,1.0f)) );
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, totalFolders * 36);

	lineMan.Render(cam);
	pointMan.Render(cam);
}

void DirectoryTree::DestroyTree(Folder* root){
	if(root != nullptr){
		// Extend downwards if possible
		for(std::unordered_map<std::string, Folder*>::iterator itr = root->subfolders.begin(); itr != root->subfolders.end(); itr++){
			DestroyTree(itr->second);
		}

		// Clean up the files in the folder
		for(std::unordered_map<std::string, File*>::iterator itr = root->files.begin(); itr != root->files.end(); itr++){
			if( itr->second != nullptr){
				delete itr->second;
			}
		}
		root->files.clear();

		//std::cout << "Deleting " << GetPath(root) << std::endl;
		// Clean up the subfolders in the folder
		for(std::unordered_map<std::string, Folder*>::iterator itr = root->subfolders.begin(); itr != root->subfolders.end(); itr++){
			if( itr->second != nullptr){
				delete itr->second;
			}
		}
		root->subfolders.clear();
	}
}

void DirectoryTree::ResetTree(){
	DestroyTree(root);
	if(root != nullptr){
		delete root;
		root = nullptr;
	}
	totalFolders = 0;
	totalFiles = 0;
	totalFileSize = 0;
	treeIsBuilding = false;
	treeIsBuilt = false;
	killThread = false;
	meshIsConstructed = false;
}

DirectoryTree::DirectoryTree(){
	ResetTree();
}

DirectoryTree::DirectoryTree(std::string path){
	ResetTree();
	TEST_ANGLE = .1f;
	SetPath(path);
}

DirectoryTree::~DirectoryTree(){
	std::cout << "Destroying tree with root: " << rootPath << std::endl;
	// Wait for the build thread to complete if it is running.
	std::cout << "Killing threads... ";
	killThread = true;
	buildThread.join();
	std::cout << "They're dead, Jim." << std::endl;
	std::cout << "Destroying tree... ";
	DestroyTree(root);
	if(root != nullptr)
		delete root;
	std::cout << "Tree destroyed." << std::endl;

	if(points != nullptr)
		delete[] points;
}



Folder* DirectoryTree::GetRoot(){
	return root;
}

int DirectoryTree::GetDepth(Folder* folder){

	Folder* tempF = folder;
	int depth = 0;

	while(tempF->parent != nullptr){
		tempF = tempF->parent;
		depth++;
	}

	return depth;
}

void DirectoryTree::Update(){
	if(treeIsBuilt && !meshIsConstructed){
		BuildGeometry();
		meshIsConstructed = true;

#if __DEBUG
		std::cout << "Mesh built for tree." << std::endl;
#endif // __DEBUG
	}

	if(Keys::T){
		TEST_ANGLE += .1f;
		std::cout << "Test angle: " << TEST_ANGLE << std::endl;
		BuildGeometry();
	}
}

bool DirectoryTree::Done(){
	return treeIsBuilt && meshIsConstructed;
}