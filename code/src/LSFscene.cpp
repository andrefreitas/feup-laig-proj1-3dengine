#include "LSFscene.h"
#include "CGFaxis.h"
#include "CGFapplication.h"
#include <math.h>
#include "LSFrender.h"
#include "LSFinterface.h"
#include <iostream>
using namespace std;
float pi = acos(-1.0);
float deg2rad=pi/180.0;

#include "CGFappearance.h"

CGFappearance *mat1;

LSFscene::LSFscene(){}

LSFscene::LSFscene(char* argv[]){
	if(argv[1] == NULL) inputScene=(char*)"default.lsf";
	else inputScene = argv[1];
}

LSFscene::~LSFscene(){

}

void LSFscene::init()
{
	// By default is false
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);

	// Enables textures
	glEnable ( GL_TEXTURE_2D );

	// Sets up some lighting parameters
	 // Define ambient light

	// Defines a default normal
	glNormal3f(0,0,1);

	// Initializate the parser
	sceneParser = new LSFparser(inputScene);

	// Set the default appearance
	defaultAppearance=new CGFappearance();
	float color[4]={0.6,0.6,0.6,0.6};
	defaultAppearance->setAmbient(color);
	defaultAppearance->setDiffuse(color);
	defaultAppearance->setSpecular(color);
	defaultAppearance->setShininess(0.5);

	// Get Global configurations
	setGlobals();
	// Get the Appearances
	sceneParser->getAppearances(appearances);
	// Get the lights
	sceneParser->getLights(lights,lights_enabled, lights_local, lights_doublesided,ambient);
	// Lights config
	if(lights_enabled) glEnable(GL_LIGHTING);
	if(lights_doublesided) glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	if(lights_local) glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
	// Get the nodes
	sceneParser->getNodes(nodes,rootNode);
	// Get the cameras info
	sceneParser->getCameras(cameras);

	initCameras();

}

map<string, LSFlight*> * LSFscene::getLights(){
	return &lights;
}

map<string, LSFcamera*> * LSFscene::getCameras(){
	return &cameras;
}

void LSFscene::initCameras()
{
	map<string,LSFcamera*>::iterator it;
	for(it = cameras.begin(); it != cameras.end(); it++){

		(*it).second->camera = new CGFcamera();
		(*it).second->camera->translate(0, -(*it).second->fromX);
		(*it).second->camera->translate(1, -(*it).second->fromY);
		(*it).second->camera->translate(2, -(*it).second->_far);


		if((*it).second->fromY != (*it).second->toY && (*it).second->angle != 0){
					(*it).second->camera->rotate(0, (*it).second->angle);
		}
		else if(fabs((*it).second->fromY - (*it).second->toY) >= 5){
			(*it).second->camera->translate(2, (*it).second->fromZ);
			(*it).second->camera->rotate(0, 90);
		}
	}

	//mudar aqui a camara até se criar a interface
	it = cameras.begin();
	activeCamera = (*it).second->id;
	if(DEBUGMODE) cout << activeCamera << endl;
}

void LSFscene::activateCamera(string id)
{
	activeCamera = cameras[id]->id;
	cout << "activeCamera = " << id << endl;
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

//	CGFscene::activeCamera->applyView();
	cameras[activeCamera]->camera->applyView();


	// ---- BEGIN lights section
	map<string,LSFlight*>::iterator it;
	for (it=lights.begin();it!=lights.end(); it++){
		if((*it).second->enabled){
			(*it).second->light->enable();
			(*it).second->light->draw();
		}
		else{
			(*it).second->light->disable();
		}
	}

	// ---- END lights section

	// Draw axis
	axis.draw();

	// ---- END Background, camera and axis setup
	stack<CGFappearance*> appearancesStack;
	appearancesStack.push(defaultAppearance);
	LSFrender::render(nodes,rootNode,appearances,appearancesStack);


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
