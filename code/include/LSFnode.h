#ifndef LSFPARSER_H
#define LSFPARSER_H
#include <stack>

// Classes for transformations
enum Axis{ X, Y, Z};
class Transform{
	public:
		virtual void apply();
		virtual ~Transform();
};

class Translate:Transform{
	protected:
		float x,y,z;
	public:
		Translate(float x,float y, float z);
		void apply();

};

class Rotate:Transform{
	protected:
		float angle;
		Axis axis;
	public:
		Rotate(axis,angle);
		void apply();
};

class Scale:Transform{
	protected:
		float scaleX;
		float scaleY;
	public:
		Scale(scaleX,scaleY);
		void apply();

};

// The node
class LSFnode{
	protected:
		stack<Transform> transforms;
};

#endif
