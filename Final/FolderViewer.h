#ifndef _FOLDER_VIEWER_H
#define _FOLDER_VIEWER_H

#include "Camera.h"
#include "Keys.h"
#include "Time.h"
#include "Folder.h"

class FolderViewer {
private:
	bool freeLook;
	Folder* targetFolder;
	Camera* cam;

	double lastchanged;
	double lastTime;

	void PrintLookingAt();
public:

	FolderViewer();
	~FolderViewer();

	void Update();

	void SetCamera(Camera* c);
	void SetTarget(Folder* f);
	bool HasTarget();
	void FreeLook(bool canFreelook);
};

#endif // _FOLDER_VIEWER_H