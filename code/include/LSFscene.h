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
	map<string,LSFlight*> lights;
	map<string, LSFcamera*> cameras;
	CGFappearance* defaultAppearance;
	string rootNode;
	bool lights_enabled, lights_local, lights_doublesided;
	float ambient[4];
	string activeCamera;
public:
	LSFscene();
	LSFscene(char* argv[]);
	~LSFscene();

	void init();
	void display();
	virtual void initCameras();
	void activateCamera(string id);
	// -----
	void setGlobals();

	map<string, LSFlight*> * getLights();
	map<string, LSFcamera*> * getCameras();
};

#endif
