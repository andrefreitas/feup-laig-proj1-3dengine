#ifndef LSFPARSER_H
#define LSFPARSER_H

#include <map>

#include "tinyxml.h"

class lsfParser:{
	private:
		
	public:
		void readGlobasl();
		void readCameras();
		void readLighting();
		void readAppearances();
		void readGraph();
}