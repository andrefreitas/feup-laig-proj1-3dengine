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
//	map<const char*,CGFappearance*> appearances;
	map<string,LSFnode*> nodes;
	string rootNode;

public:
	LSFscene(char* argv[]);
	~LSFscene();

	void init();
	void display();
	// -----
	void setGlobals();
};

#endif
