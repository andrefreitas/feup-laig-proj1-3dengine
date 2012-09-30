#ifndef TPSCENE_H
#define TPSCENE_H
#include "lsfParser.h"
#include "CGFscene.h"
#include "CGFappearance.h"
#include <vector>

class LSFscene : public CGFscene
{
	char* inputScene;
	lsfParser *sceneParser;
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
