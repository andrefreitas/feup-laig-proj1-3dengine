#ifndef LSFINTERFACE_H_
#define LSFINTERFACE_H_

#include "CGFinterface.h"
#include "LSFscene.h"

class LSFinterface: public CGFinterface
{
	private:
		LSFscene *scene;
		struct globalsData *globals;
		map<string,LSFlight*> *lights;
		map<string, LSFcamera*> *cameras;

		int camerasGroup;
		int polygonalMode;

		GLenum face;
		GLenum mode;

	public:
		LSFinterface(LSFscene *scene);
		~LSFinterface();

		virtual void initGUI();
		virtual void processGUI(GLUI_Control *ctrl);

};


#endif /* LSFINTERFACE_H_ */
