#include "LSFscene.h"
#include "CGFaxis.h"
#include "CGFapplication.h"
#include <math.h>
#include "LSFrender.h"
#include <iostream>
using namespace std;
float pi = acos(-1.0);
float deg2rad=pi/180.0;

#include "CGFappearance.h"

CGFappearance *mat1;

LSFscene::LSFscene(char* argv[]){
	if(argv[1] == NULL)
		inputScene = "teste.lsf";
	else
		inputScene = argv[1];
}

LSFscene::~LSFscene(){

}

void LSFscene::init()
{
	// Enables lighting computations
	glEnable(GL_LIGHTING);

	// Sets up some lighting parameters
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, CGFlight::background_ambient);  // Define ambient light
	
	// Declares and enables a light
	float light0_pos[4] = {4.0, 6.0, 5.0, 1.0};
	CGFlight* light0 = new CGFlight(GL_LIGHT0, light0_pos);
	light0->enable();

	// Defines a default normal
	glNormal3f(0,0,1);

	// Initializate the parser
	sceneParser = new LSFparser(inputScene);

	// Get Global configurations
	setGlobals();
	vector<CGFcamera*> cameras;
	vector<CGFappearance*> appearances;
	sceneParser->getCameras(cameras);
	sceneParser->getAppearances(appearances);
	sceneParser->getLightings();
	sceneParser->getNodes(nodes,rootNode);
}

void LSFscene::display()
{

	// ---- BEGIN Background, camera and axis setup
	
	// Clear image and depth buffer everytime we update the scene
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// Initialize Model-View matrix as identity (no transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Apply transformations corresponding to the camera position relative to the origin
	CGFscene::activeCamera->applyView();

	// Draw axis
	axis.draw();

	// ---- END Background, camera and axis setup
	LSFrender::render(nodes,rootNode);

	// ---- BEGIN Primitive drawing section
	// ---- END Primitive drawing section

	// We have been drawing in a memory area that is not visible - the back buffer, 
	// while the graphics card is showing the contents of another buffer - the front buffer
	// glutSwapBuffers() will swap pointers so that the back buffer becomes the front buffer and vice-versa
	glutSwapBuffers();
}

void LSFscene::setGlobals(){
	struct globalsData globals;
	sceneParser->getGlobals(&globals);

	// Set Background
	glClearColor(globals.background_r, globals.background_g, globals.background_b, globals.background_a);

	// Set polygon mode and shading
	GLenum  face=GL_FRONT_AND_BACK, mode=GL_FILL;
	if(strcmp(globals.polygon_mode,"line")==0) mode=GL_LINE;
	else if (strcmp(globals.polygon_mode,"point")==0) mode=GL_POINT;
	glPolygonMode(face, mode);

	// Set cullings params
	if(strcmp(globals.culling__frontfaceorder,"CW")==0) glFrontFace(GL_CW);
	face=GL_BACK;
	if(strcmp(globals.culling_cullface,"front")==0) face=GL_FRONT;
	if(strcmp(globals.culling_cullface,"both")==0) face=GL_FRONT_AND_BACK;
	glCullFace(face);
	if(globals.culling_enabled) glEnable(GL_CULL_FACE);
	else glDisable(GL_CULL_FACE);

}
