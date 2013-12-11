#ifndef _FOLDER_VIEWER_H
#define _FOLDER_VIEWER_H

#include "Camera.h"
#include "Keys.h"
#include "Time.h"
#include "Folder.h"
#include "Text2D.h"
#include <string>
#include <algorithm>

class FolderViewer {
private:
	bool freeLook;
	Folder* targetFolder;
	int curFolder;
	Camera* cam;

	double lastchanged;
	double lastTime;

	void PrintLookingAt();
public:

	FolderViewer();
	~FolderViewer();

	void Update();
	void Render();

	void SetCamera(Camera* c);
	void SetTarget(Folder* f);
	bool HasTarget();
	void FreeLook(bool canFreelook);
};

#endif // _FOLDER_VIEWER_H