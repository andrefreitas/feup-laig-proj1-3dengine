#ifndef TPSCENE_H
#define TPSCENE_H
#include "lsfParser.h"
#include "CGFscene.h"
#include "CGFappearance.h"
#include <vector>

class LSFscene : public CGFscene
{
	lsfParser *sceneParser;
	vector<CGFcamera*> cameras;
public:
	void init();
	void display();
	// -----
	void setGlobals();
};

#endif
