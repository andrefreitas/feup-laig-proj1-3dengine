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
	addColumn();


	GLUI_Panel *movingCamera = addPanel("camera Control", 1);
	addPanelToPanel(movingCamera, "rotate");
	addColumnToPanel(movingCamera);
	addPanelToPanel(movingCamera, "translate");

	//fica dentro do painel camera control
	GLUI_RadioGroup* rotate = addRadioGroupToPanel((GLUI_Panel*)movingCamera->first_child(), &rotateAxis, lights->size()+1);
	addRadioButtonToGroup(rotate, "x");
	addRadioButtonToGroup(rotate, "y");
	addRadioButtonToGroup(rotate, "z");

	//fica dentro do painel camera control
	GLUI_RadioGroup* translate = addRadioGroupToPanel((GLUI_Panel*)movingCamera->last_child(), &translateAxis, lights->size()+2);
	addRadioButtonToGroup(translate, "x");
	addRadioButtonToGroup(translate, "y");
	addRadioButtonToGroup(translate, "z");


	//para numerar os elementos da interface
	int i = 0;

	map<string, LSFlight*>::iterator itL;
	for(itL = lights->begin(); itL != lights->end(); itL++, i++){
		addButtonToPanel(lightsPanel,(char*)(*itL).second->id.c_str(),i);
		(*itL).second->lightNum = i;
		if(DEBUGMODE) cout << *(&(*itL).second->lightNum) << endl;
	}

	GLUI_RadioGroup* radioGroup = addRadioGroupToPanel(camerasPanel, &camerasGroup, lights->size());
	map<string, LSFcamera*>::iterator itC;
	for(itC = cameras->begin(), i = 0; itC != cameras->end(); itC++, i++){
		GLUI_RadioButton* button = addRadioButtonToGroup(radioGroup, (char*)(*itC).second->id.c_str());
		(*itC).second->cameraNum = i;
		if(DEBUGMODE) cout << *(&(*itC).second->cameraNum) << endl;
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
		if((*itC).second->cameraNum == camerasGroup)
			scene->activateCamera((*itC).first);
	}

}


