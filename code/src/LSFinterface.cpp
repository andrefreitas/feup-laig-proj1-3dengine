#include "LSFinterface.h"

LSFinterface::LSFinterface()
{
	testVar=0;
	light0 = 1;
    light1 = 1;
    light2 = 1;
    light3 = 1;
    clock = 1;
}

LSFinterface::~LSFinterface(){}


//void LSFinterface::processKeyboard(unsigned char key, int x, int y)
//{
//	// Uncomment below if you would like to process the default keys (e.g. 's' for snapshot, 'Esc' for exiting, ...)
//	 CGFinterface::processKeyboard(key, x, y);
//
//	switch(key)
//	{
//		case 'a':
//		{
//			// This is an example of accessing the associated scene
//			// To test, create the function toggleSomething in your scene to activate/deactivate something
//			((LSFscene *) scene)->toggleSomething();
//			break;
//		}
//		case 'j':
//		case 'J': ((LightingScene *) scene)->robot->rotate(5); break;
//
//		case 'l':
//		case 'L': ((LightingScene *) scene)->robot->rotate(-5); break;
//
//		case 'i':
//		case 'I': ((LightingScene *) scene)->robot->move(1); break;
//
//		case 'k':
//		case 'K': ((LightingScene *) scene)->robot->move(-1); break;
//	}
//}

void LSFinterface::initGUI()
{
	// Check CGFinterface.h and GLUI documentation for the types of controls available
	//GLUI_Panel *varPanel= addPanel("Group", 1);
	//addSpinnerToPanel(varPanel, "Val 1(interface)", 2, &testVar, 1);
	GLUI_Panel *lightPanel= addPanel("Lights", 1);
//	addColumn();
//	GLUI_Panel *clockPanel= addPanel("Clock", 1);
//	addColumn();
//	GLUI_Panel *robotPanel= addPanel("Robot", 1);


	addCheckboxToPanel(lightPanel, "Light0 ON/OFF", &light0, 0);
	addCheckboxToPanel(lightPanel, "Light1 ON/OFF", &light1, 1);
	addCheckboxToPanel(lightPanel, "Light2 ON/OFF", &light2, 2);
	addCheckboxToPanel(lightPanel, "Light3 ON/OFF", &light3, 3);

//	addCheckboxToPanel(clockPanel, "start/stop  ", &clock, 4);

//	listbox_item_id = 3;
////	Textures = addListboxToPanel(robotPanel, "Textures", &texturas, 5);
//	Textures->add_item(0, "Textura 1");
//	Textures->add_item(1, "Textura 2");
//	Textures->add_item(2, "Textura 3");
//	Textures->set_int_val (0);
//
//	GLUI_RadioGroup* mode= addRadioGroup(&robot, 6);
//	mode->set_int_val(0);
//	GLUI_RadioButton* textured = addRadioButtonToGroup(mode, "textured");
//	GLUI_RadioButton* wireframe = addRadioButtonToGroup(mode, "wireframe");

//	addRadioGroupToPanel(robotPanel, &modo, 6);


	// You could also pass a reference to a variable from the scene class, if public
	//addSpinnerToPanel(varPanel, "Val 2(scene)", 2, &(((LightingScene*) scene)->sceneVar), 2);


}

void LSFinterface::processGUI(GLUI_Control *ctrl)
{

//	switch ()
//	{
//		case 0:
//		{
//			for(int)
//			if(((LSFscene *) scene)->light0->light_ON)
//			{
//				((LightingScene *) scene)->light0->light_ON = false;
//				((LightingScene *) scene)->light0->disable();
//			}
//			else{
//				((LightingScene *) scene)->light0->light_ON = true;
//				((LightingScene *) scene)->light0->enable();
//			}
//			break;
//		}
//
//		case 1:
//		{
//			printf ("light1: %d\n", light1);
//			if(((LightingScene *) scene)->light1->light_ON)
//			{
//				((LightingScene *) scene)->light1->light_ON = false;
//				((LightingScene *) scene)->light1->disable();
//			}
//			else{
//				((LightingScene *) scene)->light1->light_ON = true;
//				((LightingScene *) scene)->light1->enable();
//			}
//			break;
//		}
//
//		case 2:
//		{
//			printf ("light2: %d\n", light2);
//			if(((LightingScene *) scene)->light2->light_ON)
//			{
//				((LightingScene *) scene)->light2->light_ON = false;
//				((LightingScene *) scene)->light2->disable();
//			}
//			else{
//				((LightingScene *) scene)->light2->light_ON = true;
//				((LightingScene *) scene)->light2->enable();
//			}
//			break;
//		}
//
//		case 3:
//		{
//			printf ("light3: %d\n", light3);
//			if(((LightingScene *) scene)->light3->light_ON)
//			{
//				((LightingScene *) scene)->light3->light_ON = false;
//				((LightingScene *) scene)->light3->disable();
//			}
//			else{
//				((LightingScene *) scene)->light3->light_ON = true;
//				((LightingScene *) scene)->light3->enable();
//			}
//			break;
//		}
//
//		case 4:
//		{
//			printf ("clock: %d\n", clock);
//			if(((LightingScene *) scene)->clock->getState())
//			{
//				((LightingScene *) scene)->clock->setState(false);
//			}
//			else{
//				((LightingScene *) scene)->clock->setState(true);
//			}
//			break;
//		}
//		case 5:
//		{
//			printf ("robotTextura: %d\n", Textures->get_int_val());
//			if(((LightingScene *) scene)->robot->isTextured())
//			{
//				switch (Textures->get_int_val())
//				{
//				case 0: ((LightingScene *) scene)->robot->setAppearanceOption(0); break;
//				case 1: ((LightingScene *) scene)->robot->setAppearanceOption(1); break;
//				case 2: ((LightingScene *) scene)->robot->setAppearanceOption(2); break;
//				}
//			}
//			break;
//		}
//		case 6:
//		{
//			printf ("robotMode: %d\n", robot);
//			if(((LightingScene *) scene)->robot->isTextured())
//			{
//				((LightingScene *) scene)->robot->setMode(false);
//			}
//			else{
//				((LightingScene *) scene)->robot->setMode(true);
//			}
//			break;
//		}
//	};
}


