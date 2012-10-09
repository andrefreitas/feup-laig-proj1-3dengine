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



void LSFinterface::initGUI()
{



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


