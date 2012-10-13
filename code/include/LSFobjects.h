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
		vector<LSFvertex> uvCoords;
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
		bool isspotLight;
		int lightNum;
		int isActivated;
		CGFlight* light;
		float spotExponent;
};

class LSFcamera{
	private:
		float position[3]; //= {0.0,0.0,-25.0};
		float target[3]; //= {0.0,0.0,0.0};
		float rotation[3];// = {0.0,0.0,0.0};

		enum CAMERA_MODE { EXAMINE_MODE, WALK_MODE, TARGET_MODE };

		CAMERA_MODE mode;

	public:
		string id;
		string view;
		bool initial;
		float _near, _far, angle;
		float left, right, top, bottom;
		float fromX, fromY, fromZ;
		float toX, toY, toZ;

		int cameraNum;
		int isActivated;

		CGFcamera* camera;

		void setPosition(float x, float y, float z);
		void setTarget(float x, float y, float z);
		void setStartRotation();

		void translate(int axis, float value);

		void updateProjectionMatrix(int width, int height);
		void applyView();
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
