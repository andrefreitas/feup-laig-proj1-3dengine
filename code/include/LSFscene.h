#ifndef TPSCENE_H
#define TPSCENE_H
#include "lsfParser.h"
#include "CGFscene.h"

class LSFscene : public CGFscene
{
	lsfParser *sceneParser;
public:
	void init();
	void display();
};

#endif
