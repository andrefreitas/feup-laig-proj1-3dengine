#ifndef LSFNODE_H
#define LSFNODE_H
#include <map>
#include <string>
#include <CGFappearance.h>
#include "CGFlight.h"
// Class for saving the primitive params
enum PrimitiveType{ rectangle,triangle,cylinder,sphere,torus};
class Primitive{
	public:
		PrimitiveType type;
		map<string,float> attr;
		Primitive(PrimitiveType type):type(type){};

};

enum TransformType{ translate,rotateX,rotateY,rotateZ,scale};
class Transform{
	public:
	    TransformType type;
		map<string,float> attr;
		Transform(TransformType type):type(type){};

};

class LSFlight {
	public:
		string id;
		bool enabled;
		CGFlight* light;
};

// The node in the graph
class LSFnode{
	public:
		char *id;
		float transformMatrix[16]; // When the node is parsed, the transformations are put in a stack
		string appearance; // Reference to the map of appearances
		vector<Primitive> childPrimitives; // All the basic primitives
		vector<string> childNoderefs; // All the child nodes

};
#endif
