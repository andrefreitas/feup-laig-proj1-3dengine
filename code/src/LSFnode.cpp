#include "LSFnode.h"


void LSFcamera::updateProjectionMatrix(int width, int height){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float aspectRatio = (width/(float)height);
	float xScale = 1;
	float yScale = 1;

//	if(aspectRatio < 1)
//		xScale = aspectRatio;
//	else if(aspectRatio > 1)
//		yScale = aspectRatio;

	if(view == "ortho"){
		glOrtho(left*xScale, right*xScale, bottom*yScale, top*yScale, _near, _far);
//		glOrtho(-aspectRatio*.04, aspectRatio*.04, -.04, .04, .1, 50.0);
	}
	else{
//		glFrustum(-aspectRatio*.04, aspectRatio*.04, -(fabs(fromY)+fabs(toY)), (fabs(fromY)+fabs(toY)), _near, _far);
		gluPerspective(angle, (width/(float)height), _near, _far);
	}

	glMatrixMode(GL_MODELVIEW);

}
