#ifndef LSFINTERFACE_H_
#define LSFINTERFACE_H_

#include "CGFinterface.h"
#include "LSFscene.h"

class LSFinterface: public CGFinterface
{
	private:
		LSFscene *scene;
		map<string,LSFlight*> *lights;
		map<string, LSFcamera*> *cameras;

		int camerasGroup;

	public:
		LSFinterface(LSFscene *scene);
		~LSFinterface();

		virtual void initGUI();
		virtual void processGUI(GLUI_Control *ctrl);

};


#endif /* LSFINTERFACE_H_ */
