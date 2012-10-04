#ifndef LSFNODE_H
#define LSFNODE_H
#include <map>
#include <string>
#include <CGFappearance.h>

// Class for getting the matrix of a certain transformation
enum Axis{ X, Y, Z};
class TransformMatrix{
	public:
		static float **getRotation(Axis axis, float deg);
		static float **getTranslate(float x, float y, float z);
		static float **getScale(float x, float y, float z);
		static float **mult(float **m1, float **m2);
};

// Class for saving the primitive params
enum PrimitiveType{ rectangle,triangle,cylinder,sphere,torus};
class Primitive{
	public:
		PrimitiveType type;
		map<const char*,float> attr;
		Primitive(PrimitiveType type):type(type){};

};

enum TransformType{ translate,rotateX,rotateY,rotateZ,scale};
class Transform{
	public:
	    TransformType type;
		map<const char*,float> attr;
		Transform(TransformType type):type(type){};

};

// The node in the graph
class LSFnode{
	public:
		char *id;
		float transformMatrix[16]; // When the node is parsed, the transformations are put in a stack
		CGFappearance *appearance; // Fetches the loaded appearance pointer
		vector<Primitive> childPrimitives; // All the basic primitives
		vector<string> childNoderefs; // All the child nodes

};
#endif
