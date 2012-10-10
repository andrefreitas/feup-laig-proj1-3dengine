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


	//	GLUI_Checkbox* addCheckbox(char* name, int* value = (int*) 0, int id = -1);
	//	GLUI_Checkbox* addCheckboxToPanel(GLUI_Panel *p,char* name, int* value= (int*) 0,int id = -1);
	//
	//	GLUI_Button* addButton(char* name,int id = -1);
	//	GLUI_Button* addButtonToPanel(GLUI_Panel *p,char* name,int id = -1);
	//
	//	void addColumn();
	//	void addColumnToPanel(GLUI_Panel *p);
	//
	//	GLUI_EditText* addEditText(char* name, char* var = (char*) 0,int id = -1);
	//	GLUI_EditText* addEditText(char* name, int* var = (int*) 0,int id = -1);
	//	GLUI_EditText* addEditText(char* name, float* var = (float*) 0,int id = -1);
	//	GLUI_EditText* addEditTextToPanel(GLUI_Panel *p,char* name, char* var = (char*) 0,int id = -1);
	//	GLUI_EditText* addEditTextToPanel(GLUI_Panel *p,char* name, int* var= (int*) 0,int id = -1);
	//	GLUI_EditText* addEditTextToPanel(GLUI_Panel *p,char* name, float* var= (float*) 0,int id = -1);
	//
	//	GLUI_Listbox* addListbox(char* name, int* var= (int*) 0, int id = -1);
	//	GLUI_Listbox* addListboxToPanel(GLUI_Panel *p,char* name, int* var= (int*) 0, int id = -1);
	//
	//	GLUI_Panel* addPanel(char* name, int type = 1);
	//	GLUI_Panel* addPanelToPanel(GLUI_Panel *p,char* name, int type = 1);
	//
	//	GLUI_RadioButton* addRadioButtonToGroup(GLUI_RadioGroup * group, char * name);
	//
	//	GLUI_RadioGroup* addRadioGroup(int *var, int id=-1);
	//	GLUI_RadioGroup* addRadioGroupToPanel(GLUI_Panel* p,int *var= (int*) 0, int id=-1);


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
		button->init_live();
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


