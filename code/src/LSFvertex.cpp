#include "LSFvertex.h"
#include "math.h"
#include <iostream>

using namespace std;
LSFvertex::LSFvertex(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;

}
void LSFvertex::rotateY(double angleRad) {
	double xtemp = x; // one does not simply use the changed x in the z equation
	x = x * cos(angleRad) + z * sin(angleRad);
	z = -xtemp * sin(angleRad) + z * cos(angleRad);

}

void LSFvertex::rotateZ(double angleRad){
	double xtemp=x;
	x = x * cos(angleRad) - y * sin(angleRad);
	y = xtemp * sin(angleRad) + y * cos(angleRad);
}

void LSFvertex::translate(double dx, double dy, double dz){
	x=x+dx;
	y=y+dy;
	z=z+dz;
}
mySemiRect::mySemiRect(LSFvertex *begin, LSFvertex *end, double divideStep) {
	this->begin = begin;
	this->end = end;
	this->divideStep = divideStep;

	deltaX = end->x - begin->x;
	deltaX = deltaX / divideStep;

	deltaY = end->y - begin->y;
	deltaY = deltaY / divideStep;

	deltaZ = end->z - begin->z;
	deltaZ = deltaZ / divideStep;
}

LSFvertex mySemiRect::getVertexT(int t) {
	return LSFvertex(begin->x + deltaX * t, begin->y + deltaY * t,
			begin->z + deltaZ * t);
}

LSFvertex computeNormalNewel(vector<LSFvertex> vertexList) {
	// initial normal
	LSFvertex normal(0, 0, 0);
	for (int unsigned i = 0; i < vertexList.size(); i++) {
		LSFvertex current = vertexList[i];
		LSFvertex next = vertexList[(i + 1) / vertexList.size()];
		normal.x = normal.x + ((current.y - next.y) * (current.z + next.z));
		normal.y = normal.y + ((current.z - next.z) * (current.x + next.x));
		normal.z = normal.z + ((current.x - next.x) * (current.y + next.y));
	}
	return normal;

}

vector<LSFvertex> computeTriangleUV(vector<LSFvertex> vertexList){
	LSFvertex vectorA; float normA;
	// Compute A
	vectorA.x=vertexList[1].x-vertexList[0].x;
	vectorA.y=vertexList[1].y-vertexList[0].y;
	vectorA.z=vertexList[1].z-vertexList[0].z;
	normA=sqrt(vectorA.x*vectorA.x + vectorA.y*vectorA.y + vectorA.z*vectorA.z);

	LSFvertex vectorB; float normB;
	// Compute B
	vectorB.x=vertexList[2].x-vertexList[1].x;
	vectorB.y=vertexList[2].y-vertexList[1].y;
	vectorB.z=vertexList[2].z-vertexList[1].z;
	normB=sqrt(vectorB.x*vectorB.x + vectorB.y*vectorB.y + vectorB.z*vectorB.z);

	LSFvertex vectorC; float normC;
	// Compute C
	vectorC.x=vertexList[2].x-vertexList[0].x;
	vectorC.y=vertexList[2].y-vertexList[0].y;
	vectorC.z=vertexList[2].z-vertexList[0].z;
	normC=sqrt(vectorC.x*vectorC.x + vectorC.y*vectorC.y + vectorC.z*vectorC.z);

	// Compute scale factor
	float sideA, sideB,sideC;
	float scale=1/(float)normA;
	sideA=1.0;
	sideB=scale*normB;
	sideC=scale*normC;

	// UV
	LSFvertex uv1(0,0,0);
	LSFvertex uv2(1,0,0);

}

