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
	camerasElement=lsfElement->FirstChildElement( "cameras" );

	// Parse
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

void lsfParser::getCameras(vector<CGFcamera*> &cameras){
	TiXmlElement *node=camerasElement->FirstChildElement();
	int counter=0;
	const char *camera_id, *initial;
	initial=camerasElement->Attribute("initial");
	float camera_near,camera_far,camera_angle,camera_left, camera_right, camera_top,camera_bottom;
	float camera_fromX,camera_fromY,camera_fromZ;
	float camera_toX,camera_toY,camera_toZ;
	cout << "\n--- Cameras: " << initial << " ---";
	// Loop trough cameras
	while(node){
		counter++;
		const char* val=node->Value();
		// -->
		if(strcmp(val,"ortho")==0){
			camera_id=node->Attribute("id");
			node->QueryFloatAttribute("near",&camera_near);
			node->QueryFloatAttribute("far",&camera_far);
			node->QueryFloatAttribute("left",&camera_left);
			node->QueryFloatAttribute("right",&camera_right);
			node->QueryFloatAttribute("top",&camera_top);
			node->QueryFloatAttribute("bottom",&camera_bottom);

			if(DEBUGMODE){
				float attributes[]={camera_near,camera_far,camera_left,camera_right,camera_top,camera_bottom};
				cout << "\n\tOrtho: " << camera_id << " ";
				for(int unsigned i=0; i<6; i++) cout << attributes[i] << " ";
				cout << endl;
			}
		}
		else{
			camera_id=node->Attribute("id");
			node->QueryFloatAttribute("near",&camera_near);
			node->QueryFloatAttribute("far",&camera_far);
			node->QueryFloatAttribute("angle",&camera_angle);

			// From
			TiXmlElement *from, *to;
			from=node->FirstChildElement("from");
			from->QueryFloatAttribute("x",&camera_fromX);
			from->QueryFloatAttribute("y",&camera_fromY);
			from->QueryFloatAttribute("z",&camera_fromZ);

			// To
			to=node->FirstChildElement("to");
			to->QueryFloatAttribute("x",&camera_toX);
			to->QueryFloatAttribute("y",&camera_toY);
			to->QueryFloatAttribute("z",&camera_toZ);

			if(DEBUGMODE){
				float attributes[]={camera_near,camera_far,camera_angle};
				cout << "\n\tPerspective: " << camera_id << " ";
				for(int unsigned i=0; i<3; i++) cout << attributes[i] << " ";
				cout << endl;
				cout << "\t\t\tFrom: " << camera_fromX << " " << camera_fromY << " " << camera_fromZ << endl;
				cout << "\t\t\tTo: " << camera_toX << " " << camera_toY << " " << camera_toZ << endl;

			}

		}

		// -->
		node=node->NextSiblingElement();
	}
}
