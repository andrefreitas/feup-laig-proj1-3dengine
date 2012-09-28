#ifndef LSFPARSER_H
#define LSFPARSER_H

#include "tinyxml.h"

class lsfParser{
	private:
		TiXmlDocument* file;

		TiXmlNode* node;

		TiXmlElement* globals;
		TiXmlElement* cameras;
		TiXmlElement* lighting;
		TiXmlElement* appearances;
		TiXmlElement* graph;
		
	public:
		lsfParser(char* file);
		~lsfParser();

		void readGlobals(TiXmlElement* globals);
		void readCameras(TiXmlElement* cameras);
		void readLighting(TiXmlElement* lighting);
		void readAppearances(TiXmlElement* appearances);
		void readGraph(TiXmlElement* graph);
		
		TiXmlElement* getGlobals();
		TiXmlElement* getCameras();
		TiXmlElement* getLighting();
		TiXmlElement* getAppearances();
		TiXmlElement* getGraph();
};

#endif
