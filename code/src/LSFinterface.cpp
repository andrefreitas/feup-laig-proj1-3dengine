#include "LSFinterface.h"
#include <iostream>

LSFinterface::LSFinterface(LSFscene *scene)
{
	this->scene = scene;
}

LSFinterface::~LSFinterface(){}



void LSFinterface::initGUI()
{

	lights = scene->getLights();
	cameras = scene->getCameras();


	GLUI_Panel *lightsPanel = addPanel("Lights", 1);
	addColumn();
	GLUI_Panel *camerasPanel = addPanel("Cameras", 1);

	//para numerar os elementos da interface
	int i = 0;

	map<string, LSFlight*>::iterator itL;
	for(itL = lights->begin(); itL != lights->end(); itL++, i++){
		addCheckboxToPanel(lightsPanel, (char*)(*itL).second->id.c_str(), &(*itL).second->isActivated, i);
		(*itL).second->lightNum = i;
		cout << *(&(*itL).second->lightNum) << endl;
	}

	map<string, LSFcamera*>::iterator itC;
	for(itC = cameras->begin(); itC != cameras->end(); itC++, i++){
		addCheckboxToPanel(camerasPanel, (char*)(*itC).second->id.c_str(), &(*itC).second->isActivated, i);
		(*itC).second->cameraNum = i;
		cout << *(&(*itC).second->cameraNum) << endl;
	}

	map<string, LSFcamera*>::iterator itC2;
	for(itC2 = cameras->begin(); itC2 != cameras->end(); itC2++){
		cout << "-------------------" << endl;
		cout << (*itC2).second->cameraNum << endl;
	}
}

void LSFinterface::processGUI(GLUI_Control *ctrl)
{
	cout << "ctrl =" << ctrl->user_id << endl;
	map<string, LSFlight*>::iterator itL;
	for(itL = lights->begin(); itL != lights->end(); itL++){
		cout << (*itL).second->lightNum << " - " << ctrl->user_id << endl;
		if((*itL).second->lightNum == ctrl->user_id){
			if((*itL).second->enabled)
				(*itL).second->enabled = false;
			else
				(*itL).second->enabled = true;

		}
	}

	map<string, LSFcamera*>::iterator itC;
	for(itC = cameras->begin(); itC != cameras->end(); itC++){
		cout << (*itC).second->cameraNum << " - " << ctrl->user_id << endl;
		if((*itC).second->cameraNum == ctrl->user_id && ctrl->user_id >= lights->size()){
			scene->activateCamera((*itC).first);
			scene->display();
			cout << "camera=" << (*itC).second->cameraNum << endl;
			cout << "GUI activeCamera = " << (*itC).first << endl;
		}
	}

}


