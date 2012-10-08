#ifndef LSFPARSER_H
#define LSFPARSER_H
#define DEBUGMODE 1
#include "tinyxml.h"
#include <vector>
#include "CGFcamera.h"
#include "LSFnode.h"
#include "CGFlight.h"
using namespace std;

struct globalsData{
	float background_r,background_g,background_b,background_a;
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

	public:
		LSFparser(char* a);
		void getGlobals(struct globalsData *globals);
		void getCameras(vector<CGFcamera*> &cameras);
		void getNodes(map<string,LSFnode*> &nodes,string &rootNode);
		void getAppearances(map<string,CGFappearance*> &appearances);
		void getLights(vector<CGFlight*> &lights, bool &enabled, bool &local, bool &doublesided,float *ambient);


};



#endif
