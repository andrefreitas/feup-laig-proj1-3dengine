/****************************************************************************
 * Author:      - André Freitas, p.andrefreitas@gmail.com
 * Author:      - Paulo Araújo, paulojmaraujo@gmail.com
 * Copyright:   - 14/10/2012, LAIG, FEUP
 ****************************************************************************/
#ifndef LSFSCENE_H
#define LSFSCENE_H
#include "LSFParser.h"
#include "CGFscene.h"
#include "CGFappearance.h"
#include <vector>
#include <map>

class LSFscene : public CGFscene
{
	char* inputScene;
	LSFparser *sceneParser;
	struct globalsData globals;
	map<string,LSFappearance*> appearances;
	map<string,LSFnode*> nodes;
	map<string,LSFlight*> lights;
	map<string, LSFcamera*> cameras;
	LSFappearance* defaultAppearance;
	string rootNode;
	bool lights_enabled, lights_local, lights_doublesided;
	float ambient[4];
	string activeCamera;

	GLenum face;
	GLenum mode;
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
	void setPolygonMode(unsigned int, unsigned int);
	void positionView(string activeCamera, int axis, float newPosition);

	struct globalsData *getGlobals();
	map<string, LSFlight*> * getLights();
	map<string, LSFcamera*> * getCameras();
};

#endif
