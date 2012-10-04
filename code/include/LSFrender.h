#ifndef LSFRENDER_H_
#define LSFRENDER_H_
#include <map>
#include <string>
#include "LSFnode.h"
#include "CGFapplication.h"
class LSFrender{
	public:
		static void render(map<string,LSFnode*> &nodes,string &rootNode);
};



#endif
