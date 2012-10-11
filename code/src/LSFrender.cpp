#include "LSFrender.h"
#include "LSFnode.h"
#include <iostream>
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

	// Process the primitives
	for (int unsigned i=0; i<nodes[rootNode]->childPrimitives.size();i++){

		Primitive primitive=nodes[rootNode]->childPrimitives[i];
		switch(primitive.type){
			case rectangle:{
				glBegin(GL_QUADS);
					glTexCoord2d(0.0,0.0); glVertex3d(primitive.attr["x1"],primitive.attr["y1"],0);
					glTexCoord2d(1.0,0.0); glVertex3d(primitive.attr["x2"],primitive.attr["y1"],0);
					glTexCoord2d(1.0,1.0); glVertex3d(primitive.attr["x2"],primitive.attr["y2"],0);
					glTexCoord2d(0.0,1.0); glVertex3d(primitive.attr["x1"],primitive.attr["y2"],0);
					// Todo: é necessário depois calcular as normais com método de Newell
				glEnd();
			}break;
			case triangle:{
				glBegin(GL_TRIANGLES);
					glTexCoord2d(0.0,0.0); glVertex3d(primitive.attr["x1"],primitive.attr["y1"],primitive.attr["z1"]);
					glTexCoord2d(1.0,0.0); glVertex3d(primitive.attr["x2"],primitive.attr["y2"],primitive.attr["z2"]);
					glTexCoord2d(0.5,1.0); glVertex3d(primitive.attr["x3"],primitive.attr["y3"],primitive.attr["z3"]);
					// Todo: é necessário depois calcular as normais com método de Newell
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
				// TODO: Como mapear texturas num torus? (é só isto que falta)
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
