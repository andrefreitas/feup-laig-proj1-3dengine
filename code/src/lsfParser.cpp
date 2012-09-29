#include <iostream>
#include "lsfParser.h"

lsfParser::lsfParser(char* a){
	// Load the File
	if(DEBUGMODE) cout << "lsfParser(" << a <<");\n";
	lsfFile= new TiXmlDocument(a);
	if(DEBUGMODE){
		cout << "1- XML open: ";
		(lsfFile->LoadFile())? cout << "OK": cout << "ERROR";
	}
	// Fetch main elements sections
	lsfElement=lsfFile->FirstChildElement( "lsf" );
	globalsElement=lsfElement->FirstChildElement( "globals" );

	// Parse
	getGlobals();
}

void lsfParser::getGlobals(){
	TiXmlElement *background=globalsElement->FirstChildElement( "background" );
	float r,g,b;
	background->QueryFloatAttribute("r",&r);
	background->QueryFloatAttribute("g",&g);
	background->QueryFloatAttribute("b",&b);
	cout << "background r= " << r <<" g= " << g << " b= " << b << endl;
}
