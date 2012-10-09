#ifndef LSFINTERFACE_H_
#define LSFINTERFACE_H_

#include "CGFinterface.h"

class LSFinterface: public CGFinterface
{
	public:
		LSFinterface();
		~LSFinterface();

		virtual void initGUI();
		virtual void processGUI(GLUI_Control *ctrl);

		int testVar;
		int light0;
		int light1;
		int light2;
		int light3;
		int clock;
		int robot;
		int modo;
		int listbox_item_id;
		int texturas;

		GLUI_Listbox *Textures;
};


#endif /* LSFINTERFACE_H_ */
