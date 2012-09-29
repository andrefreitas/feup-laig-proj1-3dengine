#ifndef LSFPARSER_H
#define LSFPARSER_H
#define DEBUGMODE 1
#include "tinyxml.h"
using namespace std;

class lsfParser{
	private:
		TiXmlDocument *lsfFile;
		TiXmlElement* lsfElement;
		TiXmlElement* globalsElement;
	public:
		lsfParser(char* a);
		void getGlobals();

};

#endif
