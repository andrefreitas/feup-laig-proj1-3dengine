#ifndef TPSCENE_H
#define TPSCENE_H
#include "LSFParser.h"
#include "CGFscene.h"
#include "CGFappearance.h"
#include <vector>

class LSFscene : public CGFscene
{
	char* inputScene;
	LSFparser *sceneParser;
	vector<CGFcamera*> cameras;
public:
	LSFscene(char* argv[]);
	~LSFscene();

	void init();
	void display();
	// -----
	void setGlobals();
};

#endif
