#include "LSFrender.h"
#include "LSFnode.h"
#include <iostream>
#include "LSFvertex.h"
using namespace std;
void LSFrender::render(map<string,LSFnode*> &nodes,string &rootNode,map<string,LSFappearance*> appearances,stack<LSFappearance*> &appearancesStack){
	if(nodes[rootNode]==0) {
		//cout << "\nNo inexistente\n";
		return;
	}
	// Transforms
	glPushMatrix();
	glMultMatrixf(nodes[rootNode]->transformMatrix);

	// Appearances
	LSFappearance *currentAppearance;
	if(nodes[rootNode]->appearance=="inherit") currentAppearance=appearancesStack.top();
	else currentAppearance=appearances[nodes[rootNode]->appearance];
	appearancesStack.push(currentAppearance);
	currentAppearance->appearance->apply();
	// Emissive
	glMaterialfv(GL_EMISSION,GL_FRONT_AND_BACK,currentAppearance->emissive);
	// Compute UV coords by ST length
	float u,v;
	u=1/(float)currentAppearance->length_s;
	v=1/(float)currentAppearance->length_t;
	// Process the primitives
	for (int unsigned i=0; i<nodes[rootNode]->childPrimitives.size();i++){

		Primitive primitive=nodes[rootNode]->childPrimitives[i];
		switch(primitive.type){
			case rectangle:{
				glNormal3f(0,0,1);
				glBegin(GL_QUADS);
					glTexCoord2d(0.0,0.0); glVertex3d(primitive.attr["x1"],primitive.attr["y1"],0);
					glTexCoord2d(u,0.0); glVertex3d(primitive.attr["x2"],primitive.attr["y1"],0);
					glTexCoord2d(u,v); glVertex3d(primitive.attr["x2"],primitive.attr["y2"],0);
					glTexCoord2d(0.0,v); glVertex3d(primitive.attr["x1"],primitive.attr["y2"],0);

				glEnd();
			}break;
			case triangle:{
				glNormal3f(primitive.normal.x/primitive.normal.x,primitive.normal.y/primitive.normal.y,primitive.normal.z/primitive.normal.z);
				glBegin(GL_TRIANGLES);
					glTexCoord2d(0,0); // don't need s and t in the first coord
					glVertex3d(primitive.attr["x1"],primitive.attr["y1"],primitive.attr["z1"]);
					glTexCoord2d(primitive.uvCoords[1].x/(float)currentAppearance->length_s,primitive.uvCoords[1].y/(float)currentAppearance->length_t);
					glVertex3d(primitive.attr["x2"],primitive.attr["y2"],primitive.attr["z2"]);
					glTexCoord2d(primitive.uvCoords[2].x/(float)currentAppearance->length_s,primitive.uvCoords[2].y/(float)currentAppearance->length_t);
					glVertex3d(primitive.attr["x3"],primitive.attr["y3"],primitive.attr["z3"]);
				glEnd();
			}break;
			case cylinder:{
				GLUquadric *a=gluNewQuadric();
				gluQuadricNormals(a,GL_SMOOTH);
				gluQuadricTexture(a,GL_TRUE);
				gluCylinder(a, primitive.attr["base"], primitive.attr["top"], primitive.attr["height"], primitive.attr["slices"], primitive.attr["stacks"]);
			} break;
			case sphere:{
				GLUquadric *a=gluNewQuadric();
				gluQuadricNormals(a,GL_SMOOTH);
				gluQuadricTexture(a,GL_TRUE);
				gluSphere(a,primitive.attr["radius"],primitive.attr["slices"],primitive.attr["stacks"]);
			}break;
			case torus:{

				glutSolidTorus(primitive.attr["inner"],primitive.attr["outer"],primitive.attr["slices"],primitive.attr["loops"]);
			} break;
		}

	}

	// Process the noderefs
	for (int unsigned i=0; i<nodes[rootNode]->childNoderefs.size();i++){
		render(nodes,nodes[rootNode]->childNoderefs[i],appearances,appearancesStack);
	}
	appearancesStack.pop();
	glPopMatrix();

}
