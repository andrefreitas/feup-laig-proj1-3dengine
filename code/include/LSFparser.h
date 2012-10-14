#ifndef LSFPARSER_H
#define LSFPARSER_H

#define DEBUGMODE 0

#include "tinyxml.h"
#include <vector>
#include "CGFcamera.h"
#include "LSFobjects.h"
#include "CGFlight.h"

using namespace std;

struct globalsData{
	float background[4];
	const char *polygon_mode, *polygon_shading;
	const char *culling__frontfaceorder, *culling_cullface; bool culling_enabled;
};

class LSFparser{
	private:
		TiXmlDocument *lsfFile;
		TiXmlElement* lsfElement;
		TiXmlElement* globalsElement;
		TiXmlElement* camerasElement;
		TiXmlElement* graphElement;
		TiXmlElement* appearancesElement;
		TiXmlElement* lightingsElement;

		int queryResult;

	public:
		LSFparser(char* a);
		void getGlobals(struct globalsData *globals);
		void getCameras(map<string, LSFcamera*> &cameras);
		void getNodes(map<string,LSFnode*> &nodes,string &rootNode);
		void getAppearances(map<string,LSFappearance*> &appearances);
		void getLights(map<string,LSFlight*> &lights, bool &enabled, bool &local, bool &doublesided,float *ambient);


};



#endif
