#include "LSFrender.h"
#include "LSFnode.h"
#include <iostream>
using namespace std;
void LSFrender::render(map<string,LSFnode*> &nodes,string &rootNode){
	// Process the primitives
	for (int unsigned i=0; i<nodes[rootNode]->childPrimitives.size();i++){
		Primitive primitive=nodes[rootNode]->childPrimitives[i];
		switch(primitive.type){
			case rectangle: glRectf(primitive.attr["x1"],primitive.attr["y1"],primitive.attr["x2"],primitive.attr["y2"]); break;
			case triangle:{
				glBegin(GL_TRIANGLES);
						glVertex3d(primitive.attr["x1"],primitive.attr["y1"],primitive.attr["z1"]);
						glVertex3d(primitive.attr["x2"],primitive.attr["y2"],primitive.attr["z2"]);
						glVertex3d(primitive.attr["x3"],primitive.attr["y3"],primitive.attr["z3"]);
					glEnd();
				break;
			}
			case cylinder:{
				GLUquadric *a=gluNewQuadric();
				gluQuadricNormals(a,GL_SMOOTH);
				gluCylinder(a, primitive.attr["base"], primitive.attr["top"], primitive.attr["height"], primitive.attr["slices"], primitive.attr["stacks"]);
			} break;
			case sphere:{
				GLUquadric *a=gluNewQuadric();
				gluQuadricNormals(a,GL_SMOOTH);
				gluSphere(a,primitive.attr["radius"],primitive.attr["slices"],primitive.attr["stacks"]);
			}break;
			case torus:{
				glutSolidTorus(primitive.attr["inner"],primitive.attr["outer"],primitive.attr["slices"],primitive.attr["loops"]);
			}
		}



	}

}
