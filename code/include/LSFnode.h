#ifndef LSFNODE_H
#define LSFNODE_H
#include <map>
#include <string>
#include <CGFappearance.h>
#include "CGFlight.h"
#include "CGFcamera.h"
#include "LSFvertex.h"
// Class for saving the primitive params
enum PrimitiveType{ rectangle,triangle,cylinder,sphere,torus};
class Primitive{
	public:
		PrimitiveType type;
		map<string,float> attr;
		LSFvertex normal;
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
		int lightNum;
		int isActivated;
		CGFlight* light;
};

class LSFcamera{
	public:
		string id;
		string view;
		float _near, _far, angle;
		float left, right, top, bottom;
		float fromX, fromY, fromZ;
		float toX, toY, toZ;

		int cameraNum;
		int isActivated;

		CGFcamera* camera;

		void updateProjectionMatrix(int width, int height);
		void applyView(); //usar lookat para as perspective
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

class LSFappearance{
	public:
		string id;
		float emissive[4];
		float length_s,length_t;
		CGFappearance* appearance;
		bool haveTexture;
};
#endif
