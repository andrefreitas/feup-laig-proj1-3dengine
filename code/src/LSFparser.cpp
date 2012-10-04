#include <iostream>
#include "LSFParser.h"
#include "LSFnode.h"
#include <map>
#include <stack>
LSFparser::LSFparser(char* a){
	// Load the File
	if(DEBUGMODE) cout << "lsfParser(" << a <<");\n";
	lsfFile= new TiXmlDocument(a);

	if(DEBUGMODE){
		cout << "1- XML open: ";
		(lsfFile->LoadFile())? cout << "OK": cout << "ERROR" << endl;
	}

	if(lsfFile->LoadFile()){
		// Fetch main elements sections
		lsfElement=lsfFile->FirstChildElement( "lsf" );
		globalsElement=lsfElement->FirstChildElement( "globals" );
		camerasElement=lsfElement->FirstChildElement( "cameras" );
		graphElement=lsfElement->FirstChildElement( "graph" );
	}
	else
		exit(1);

	// Parse
}

void LSFparser::getGlobals(struct globalsData *globals){
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

void LSFparser::getCameras(vector<CGFcamera*> &cameras){
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

void LSFparser::getNodes(){
	const char * rootid=graphElement->Attribute("rootid");
	cout << "\n--- Graph: " << rootid << " ---"<< endl;
	TiXmlElement *node=graphElement->FirstChildElement();
	vector<LSFnode*> nodes;

	while(node){
		LSFnode *pnode=new LSFnode();

		strcpy(pnode->id,node->Attribute("id")); // save in the node

		cout << "\tNode: " << node->Attribute("id") << endl;
		// (1) Transforms
		TiXmlElement *transforms=node->FirstChildElement("transforms");
		TiXmlElement *transform=transforms->FirstChildElement();
		cout << "\tTransforms:"<< endl;

		// Use openGL to compute matrix
		//glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();
		stack<Transform> transfs;
		// --->
		while(transform){
			float x,y,z,angle;
			char axis;
			const char* transVal=transform->Value();
			if(strcmp(transVal,"translate")==0){
				transform->QueryFloatAttribute("x",&x);
				transform->QueryFloatAttribute("y",&y);
				transform->QueryFloatAttribute("z",&z);
				cout << "\t\tTranslate: " << x << " " << y << " " << z << endl;
				Transform aux(translate);


			}
			else if (strcmp(transVal,"rotate")==0){
				transform->QueryFloatAttribute("angle",&angle);
				axis=*transform->Attribute("axis");
				cout << "\t\tRotate: " << angle << " " << axis << endl;

			}
			else if (strcmp(transVal,"scale")==0){
				transform->QueryFloatAttribute("x",&x);
				transform->QueryFloatAttribute("y",&y);
				transform->QueryFloatAttribute("z",&z);
				cout << "\t\tScale: " << x << " " << y << " " << z << endl;
			}
			// -->
			transform=transform->NextSiblingElement();
		}

		// (2) Appearance
		TiXmlElement *appearanceref=node->FirstChildElement("appearanceref");
		const char *appearance=appearanceref->Attribute("id");
		cout << "\tAppearance: " << appearance<< endl;

		// (3) Children
		cout << "\tChildren: " << endl;
		TiXmlElement *children=node->FirstChildElement("children");
		TiXmlElement *child=children->FirstChildElement();
		while(child){
			const char* childVal=child->Value();
			map<const char*,float> attr;
			// -->
			if(strcmp(childVal,"rectangle")==0){
				child->QueryFloatAttribute("x1",&attr["x1"]);
				child->QueryFloatAttribute("x2",&attr["x2"]);
				child->QueryFloatAttribute("y1",&attr["y1"]);
				child->QueryFloatAttribute("y2",&attr["y2"]);
				cout << "\t\trectangle: " << attr["x1"] << " " << attr["y1"] << " " << attr["x2"] << " " << attr["y2"] << endl;

			} else if(strcmp(childVal,"triangle")==0){
				child->QueryFloatAttribute("x1",&attr["x1"]);
				child->QueryFloatAttribute("x2",&attr["x2"]);
				child->QueryFloatAttribute("x3",&attr["x3"]);
				child->QueryFloatAttribute("y1",&attr["y1"]);
				child->QueryFloatAttribute("y2",&attr["y2"]);
				child->QueryFloatAttribute("y3",&attr["y3"]);
				child->QueryFloatAttribute("z1",&attr["z1"]);
				child->QueryFloatAttribute("z2",&attr["z2"]);
				child->QueryFloatAttribute("z3",&attr["z3"]);
				cout << "\t\ttriangle " << attr["x1"] << " " << attr["y1"] << " " << attr["z1"] << ""
				     << attr["x2"] << " " << attr["y2"] << " " << attr["z2"] << ""
				     << attr["x3"] << " " << attr["y3"] << " " << attr["z3"]
				     << endl;


			} else if(strcmp(childVal,"cylinder")==0){
				int slices, stacks;
				child->QueryFloatAttribute("base",&attr["base"]);
				child->QueryFloatAttribute("top",&attr["top"]);
				child->QueryFloatAttribute("height",&attr["height"]);
				child->QueryIntAttribute("slices",&slices);
				child->QueryIntAttribute("stacks",&stacks);
				cout << "\t\tcylinder " << attr["base"] << " " << attr["top"] << " " << attr["height"] << " " << slices << " " << stacks << endl;

			} else if(strcmp(childVal,"sphere")==0){
				float radius;
				int slices,stacks;
				child->QueryFloatAttribute("radius",&radius);
				child->QueryIntAttribute("slices",&slices);
				child->QueryIntAttribute("stacks",&stacks);
				cout << "\t\tsphere " << radius << " " << slices << " " << stacks << endl;

			}else if(strcmp(childVal,"torus")==0){
				float inner, outer;
				int slices, loops;
				child->QueryFloatAttribute("inner",&inner);
				child->QueryFloatAttribute("outer",&outer);
				child->QueryIntAttribute("slices",&slices);
				child->QueryIntAttribute("loops",&loops);
				cout << "\t\ttorus " <<  inner << " " << outer << " " << slices << " " << loops <<endl;

			}else if(strcmp(childVal,"noderef")==0){
				const char *idRef=child->Attribute("id");
				cout << "\t\tnoderef " << idRef << endl;

			}

			// -->
			child=child->NextSiblingElement();
		}

		// -->
		node=node->NextSiblingElement();
		cout << "\n\t-----\n";
	}

}

