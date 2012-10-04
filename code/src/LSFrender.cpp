#include "LSFrender.h"
#include "LSFnode.h"
#include <iostream>
using namespace std;
void LSFrender::render(map<string,LSFnode*> &nodes,string &rootNode){
	// Process the primitives
	for (int unsigned i=0; i<nodes[rootNode]->childPrimitives.size();i++){
		Primitive primitive=nodes[rootNode]->childPrimitives[i];
			glRectf(primitive.attr["x1"],primitive.attr["y1"],primitive.attr["x2"],primitive.attr["y2"]);


	}

}
