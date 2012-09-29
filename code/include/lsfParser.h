#ifndef LSFPARSER_H
#define LSFPARSER_H
#define DEBUGMODE 1
#include "tinyxml.h"
#include <map>
using namespace std;

struct globalsData{
	float background_r,background_g,background_b,background_a;
	const char *polygon_mode, *polygon_shading;
	const char *culling__frontfaceorder, *culling_cullface; bool culling_enabled;
};

class lsfParser{
	private:
		TiXmlDocument *lsfFile;
		TiXmlElement* lsfElement;
		TiXmlElement* globalsElement;
	public:
		lsfParser(char* a);
		void getGlobals(struct globalsData *globals);


};



#endif
