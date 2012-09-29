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
	struct globalsData globals;
	getGlobals(&globals);
}

void lsfParser::getGlobals(struct globalsData *globals){
	// Background:
	TiXmlElement *background=globalsElement->FirstChildElement( "background" );
	background->QueryFloatAttribute("r",&globals->background_r);
	background->QueryFloatAttribute("g",&globals->background_g);
	background->QueryFloatAttribute("b",&globals->background_b);
	background->QueryFloatAttribute("a",&globals->background_a);

	// Polygon
	TiXmlElement *polygon=globalsElement->FirstChildElement( "polygon" );
	globals->polygon_mode=polygon->Attribute("mode");
	globals->polygon_shading=polygon->Attribute("shading");

	// Culling
	TiXmlElement *culling=globalsElement->FirstChildElement( "culling" );
	globals->culling__frontfaceorder=culling->Attribute("frontfaceorder");
	globals->culling_cullface=culling->Attribute("cullface");
	culling->QueryBoolAttribute("enabled",&globals->culling_enabled);

	if(DEBUGMODE){
		cout << "\n--- Globals---\n";
		cout << "\tbackground: " <<globals->background_r << ' ' << globals->background_g << ' ' << globals->background_b << ' ' << globals->background_a << endl;
		cout << "\tpolygon: " <<globals->polygon_mode << ' '<< globals->polygon_shading << endl;
		cout << "\tculling: " <<globals->culling__frontfaceorder << ' '<< globals->culling_cullface <<' ' << globals->culling_enabled <<  endl;
	}
}
