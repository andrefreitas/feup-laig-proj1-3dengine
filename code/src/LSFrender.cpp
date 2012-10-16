#include "LSFrender.h"
#include "LSFobjects.h"
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
	glMaterialfv(GL_EMISSION,GL_FRONT_AND_BACK,currentAppearance->emissive);
	currentAppearance->appearance->apply();
	// Emissive
	// Compute UV coords by ST length

	// Process the primitives
	for (int unsigned i=0; i<nodes[rootNode]->childPrimitives.size();i++){

		Primitive primitive=nodes[rootNode]->childPrimitives[i];
		switch(primitive.type){
			case rectangle:{
				glNormal3f(0,0,1);
				glBegin(GL_QUADS);
					float width=primitive.attr["x2"]-primitive.attr["x1"];
					float height=primitive.attr["y2"]-primitive.attr["y1"];
					float u,v;
					u=width/(float)currentAppearance->length_s;
					v=height/(float)currentAppearance->length_t;
					glTexCoord2d(0.0,0.0); glVertex3d(primitive.attr["x1"],primitive.attr["y1"],0);
					glTexCoord2d(u,0.0); glVertex3d(primitive.attr["x2"],primitive.attr["y1"],0);
					glTexCoord2d(u,v); glVertex3d(primitive.attr["x2"],primitive.attr["y2"],0);
					glTexCoord2d(0.0,v); glVertex3d(primitive.attr["x1"],primitive.attr["y2"],0);

				glEnd();
			}break;
			case triangle:{
				glNormal3f(primitive.normal.x,primitive.normal.y,primitive.normal.z);
				glBegin(GL_TRIANGLES);

					// Compute the height and width of the triangle
					LSFvertex p1,p2,p3;
					p1=LSFvertex(primitive.attr["x1"],primitive.attr["y1"],primitive.attr["z1"]);
					p2=LSFvertex(primitive.attr["x2"],primitive.attr["y2"],primitive.attr["z2"]);
					p3=LSFvertex(primitive.attr["x3"],primitive.attr["y3"],primitive.attr["z3"]);
					float width=computeNormBetween(p1,p2);
					float sFactor=(width/currentAppearance->length_s);
					float height=computeTriangleHeight(p1,p2,p3);
					float tFactor=(height/currentAppearance->length_t);
					// -->
					glTexCoord2d(0,0); // don't need s and t in the first coord
					glVertex3d(primitive.attr["x1"],primitive.attr["y1"],primitive.attr["z1"]);

					glTexCoord2d(primitive.uvCoords[1].x*sFactor,primitive.uvCoords[1].y*tFactor);
					glVertex3d(primitive.attr["x2"],primitive.attr["y2"],primitive.attr["z2"]);

					glTexCoord2d(primitive.uvCoords[2].x*sFactor,primitive.uvCoords[2].y*tFactor);
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
