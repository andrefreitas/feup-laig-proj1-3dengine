#ifndef TPSCENE_H
#define TPSCENE_H
#include "LSFParser.h"
#include "CGFscene.h"
#include "CGFappearance.h"
#include <vector>
#include <map>
class LSFscene : public CGFscene
{
	char* inputScene;
	LSFparser *sceneParser;
	map<string,CGFappearance*> appearances;
	map<string,LSFnode*> nodes;
	vector<CGFlight*> lights;
	vector<Camera*> cameras;
	CGFappearance* defaultAppearance;
	string rootNode;
	bool lights_enabled, lights_local, lights_doublesided;
	float ambient[4];
	vector<CGFcamera*> sceneCameras;
	int active_camera;
	CGFcamera* activeCamera;
public:
	LSFscene(char* argv[]);
	~LSFscene();

	void init();
	void display();
	virtual void initCameras();
	void activateCamera(int i);
	// -----
	void setGlobals();
};

#endif
