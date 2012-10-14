/****************************************************************************
 * Author:      - André Freitas, p.andrefreitas@gmail.com
 * Author:      - Paulo Araújo, paulojmaraujo@gmail.com
 * Copyright:   - 14/10/2012, LAIG, FEUP
 ****************************************************************************/
#ifndef LSFRENDER_H_
#define LSFRENDER_H_
#include <map>
#include <string>
#include "LSFobjects.h"
#include "CGFapplication.h"
#include <stack>
class LSFrender{
	public:
		static void render(map<string,LSFnode*> &nodes,string &rootNode,map<string,LSFappearance*> appearances,stack<LSFappearance*> &appearancesStack);
};



#endif
