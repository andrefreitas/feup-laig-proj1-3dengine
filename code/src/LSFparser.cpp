#include <iostream>
#include "LSFParser.h"
#include "LSFnode.h"
#include <map>
#include <stack>
#include <CGFapplication.h>
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
		appearancesElement = lsfElement->FirstChildElement( "appearances");
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
	if(DEBUGMODE) cout << "\n--- Cameras: " << initial << " ---";
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

void LSFparser::getNodes(map<string,LSFnode*> &nodes,string &rootNode){
	const char * rootid=graphElement->Attribute("rootid");
	rootNode.assign(rootid);
	if(DEBUGMODE) cout << "\n--- Graph: " << rootid << " ---"<< endl;
	TiXmlElement *node=graphElement->FirstChildElement();

	while(node){
		LSFnode *pnode=new LSFnode();
		pnode->id=new char[100];
		strcpy(pnode->id,node->Attribute("id")); // save in the node
		if(DEBUGMODE) cout << "\tNode: " << node->Attribute("id") << endl;
		// (1) Transforms
		TiXmlElement *transforms=node->FirstChildElement("transforms");
		TiXmlElement *transform=transforms->FirstChildElement();
		if(DEBUGMODE)cout << "\tTransforms:"<< endl;
		// Compute transforms
		glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
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
				if(DEBUGMODE) cout << "\t\tTranslate: " << x << " " << y << " " << z << endl;
				glTranslatef(x,y,z);

			}
			else if (strcmp(transVal,"rotate")==0){
				transform->QueryFloatAttribute("angle",&angle);
				axis=*transform->Attribute("axis");
				if(DEBUGMODE) cout << "\t\tRotate: " << angle << " " << axis << endl;
				x=0; y=0; z=0;
				if(axis=='x') x=1; // Must be -angle Todo: wrong angle?
				if(axis=='y') y=1;
				if(axis=='z') z=1;
				glRotatef(angle,x,y,z);


			}
			else if (strcmp(transVal,"scale")==0){
				transform->QueryFloatAttribute("x",&x);
				transform->QueryFloatAttribute("y",&y);
				transform->QueryFloatAttribute("z",&z);
				if(DEBUGMODE) cout << "\t\tScale: " << x << " " << y << " " << z << endl;
				glScalef(x,y,z);

			}

			// -->
			transform=transform->NextSiblingElement();
		}
		glGetFloatv(GL_MODELVIEW_MATRIX,pnode->transformMatrix);
		glPopMatrix();

		// (2) Appearance
		TiXmlElement *appearanceref=node->FirstChildElement("appearanceref");
		const char *appearance=appearanceref->Attribute("id");
		if(DEBUGMODE) cout << "\tAppearance: " << appearance<< endl;
		//TODO: use appearances[appearanceref]

		// (3) Children
		if(DEBUGMODE) cout << "\tChildren: " << endl;
		TiXmlElement *children=node->FirstChildElement("children");
		TiXmlElement *child=children->FirstChildElement();
		while(child){
			const char* childVal=child->Value();
			map<const char*,float> attr;
			// -->

			if(strcmp(childVal,"rectangle")==0){
				Primitive prim(rectangle);
				child->QueryFloatAttribute("x1",&prim.attr["x1"]);
				child->QueryFloatAttribute("x2",&prim.attr["x2"]);
				child->QueryFloatAttribute("y1",&prim.attr["y1"]);
				child->QueryFloatAttribute("y2",&prim.attr["y2"]);
				if(DEBUGMODE) cout << "\t\trectangle: " << prim.attr["x1"] << " " << prim.attr["y1"] << " " << prim.attr["x2"] << " " << prim.attr["y2"] << endl;
				pnode->childPrimitives.push_back(prim);

			} else if(strcmp(childVal,"triangle")==0){
				Primitive prim(triangle);
				child->QueryFloatAttribute("x1",&prim.attr["x1"]);
				child->QueryFloatAttribute("x2",&prim.attr["x2"]);
				child->QueryFloatAttribute("x3",&prim.attr["x3"]);
				child->QueryFloatAttribute("y1",&prim.attr["y1"]);
				child->QueryFloatAttribute("y2",&prim.attr["y2"]);
				child->QueryFloatAttribute("y3",&prim.attr["y3"]);
				child->QueryFloatAttribute("z1",&prim.attr["z1"]);
				child->QueryFloatAttribute("z2",&prim.attr["z2"]);
				child->QueryFloatAttribute("z3",&prim.attr["z3"]);
				if(DEBUGMODE)cout << "\t\ttriangle " << prim.attr["x1"] << " " << prim.attr["y1"] << " " << prim.attr["z1"] << ""
				    << prim.attr["x2"] << " " << prim.attr["y2"] << " " << prim.attr["z2"] << ""
				    << prim.attr["x3"] << " " << prim.attr["y3"] << " " << prim.attr["z3"]
				     << endl;
				pnode->childPrimitives.push_back(prim);

			} else if(strcmp(childVal,"cylinder")==0){
				Primitive prim(cylinder);
				int slices, stacks;
				child->QueryFloatAttribute("base",&prim.attr["base"]);
				child->QueryFloatAttribute("top",&prim.attr["top"]);
				child->QueryFloatAttribute("height",&prim.attr["height"]);
				child->QueryIntAttribute("slices",&slices);
				child->QueryIntAttribute("stacks",&stacks);
				prim.attr["slices"]=slices;
				prim.attr["stacks"]=stacks;
				if(DEBUGMODE) cout << "\t\tcylinder " << prim.attr["base"] << " " << prim.attr["top"] << " " << prim.attr["height"] << " "
						<< prim.attr["slices"] << " " << prim.attr["stacks"] << endl;
				pnode->childPrimitives.push_back(prim);

			} else if(strcmp(childVal,"sphere")==0){
				Primitive prim(sphere);
				int slices,stacks;
				child->QueryFloatAttribute("radius",&prim.attr["radius"]);
				child->QueryIntAttribute("slices",&slices);
				child->QueryIntAttribute("stacks",&stacks);
				prim.attr["slices"]=slices;
				prim.attr["stacks"]=stacks;
				if(DEBUGMODE) cout << "\t\tsphere " << prim.attr["radius"] << " " << prim.attr["slices"]<< " " << prim.attr["stacks"] << endl;
				pnode->childPrimitives.push_back(prim);

			}else if(strcmp(childVal,"torus")==0){
				Primitive prim(torus);
				int slices, loops;
				child->QueryFloatAttribute("inner",&prim.attr["inner"]);
				child->QueryFloatAttribute("outer",&prim.attr["outer"]);
				child->QueryIntAttribute("slices",&slices);
				child->QueryIntAttribute("loops",&loops);
				prim.attr["slices"]=slices;
				prim.attr["loops"]=loops;
				//cout << "\t\ttorus " <<  prim.attr["inner"]<< " " << prim.attr["outer"] << " " << prim.attr["slices"] << " " << prim.attr["loops"]<<endl;
				pnode->childPrimitives.push_back(prim);
			}else if(strcmp(childVal,"noderef")==0){
				const char *idRef=child->Attribute("id");
				string st(idRef);
				pnode->childNoderefs.push_back(st);
				if(DEBUGMODE) cout << "\t\tnoderef " << st << endl;

			}

			// -->
			child=child->NextSiblingElement();
		}

		// -->
		string aux;
		aux.assign(pnode->id);
		nodes[aux]=pnode; // Add this node
		node=node->NextSiblingElement();
		if(DEBUGMODE) cout << "\n\t-----\n";
	}

}

void LSFparser::getAppearances(vector<CGFappearance*> &appearances){
	TiXmlElement *node=appearancesElement->FirstChildElement();
	int counter = 0;
	const char *appearance_id;
	float emissive_r, emissive_g, emissive_b, emissive_a;
	float ambient_r, ambient_g, ambient_b, ambient_a;
	float diffuse_r, diffuse_g, diffuse_b, diffuse_a;
	float specular_r, specular_g, specular_b, specular_a;
	float shininess_value, texture_length_s, texture_length_t;
	cout << "\n--- Aparencias  ---" << endl;
	// Loop trough cameras
	while(node){
		counter++;

		appearance_id = node->Attribute("id");
		TiXmlElement *emissive, *ambient, *diffuse, *specular, *shininess, *texture;

		emissive = node->FirstChildElement("emissive");
		if(emissive != NULL)
			if(strcmp(emissive->ValueTStr().c_str(),"emissive")==0){
				emissive->QueryFloatAttribute("r",&emissive_r);
				emissive->QueryFloatAttribute("g",&emissive_g);
				emissive->QueryFloatAttribute("b",&emissive_b);
				emissive->QueryFloatAttribute("a",&emissive_a);
			}

		ambient = node->FirstChildElement("ambient");
		if(ambient != NULL)
			if(strcmp(ambient->ValueTStr().c_str(), "ambient")==0){
				ambient->QueryFloatAttribute("r",&ambient_r);
				ambient->QueryFloatAttribute("g",&ambient_g);
				ambient->QueryFloatAttribute("b",&ambient_b);
				ambient->QueryFloatAttribute("a",&ambient_a);
			}

		diffuse = node->FirstChildElement("diffuse");
		if(diffuse != NULL)
			if(strcmp(diffuse->ValueTStr().c_str(), "diffuse")==0){
				diffuse->QueryFloatAttribute("r",&diffuse_r);
				diffuse->QueryFloatAttribute("g",&diffuse_g);
				diffuse->QueryFloatAttribute("b",&diffuse_b);
				diffuse->QueryFloatAttribute("a",&diffuse_a);
			}

		specular = node->FirstChildElement("specular");
		if(specular != NULL)
			if(strcmp(specular->ValueTStr().c_str(), "specular")==0){
				specular->QueryFloatAttribute("r",&specular_r);
				specular->QueryFloatAttribute("g",&specular_g);
				specular->QueryFloatAttribute("b",&specular_b);
				specular->QueryFloatAttribute("a",&specular_a);
			}

		shininess = node->FirstChildElement("shininess");
		if(shininess != NULL)
			if(strcmp(shininess->ValueTStr().c_str(), "shininess")==0)
				shininess->QueryFloatAttribute("value", &shininess_value);


		texture = node->FirstChildElement("texture");
		if(texture != NULL)
			if(strcmp(texture->ValueTStr().c_str(), "texture")==0){
				texture->Attribute("file");
				texture->QueryFloatAttribute("length_s", &texture_length_s);
				texture->QueryFloatAttribute("length_t", &texture_length_t);
			}



		if(DEBUGMODE){
			cout << "\n\tAparencia: " << appearance_id << " " << endl;
			if(emissive != NULL)
				cout << "emissive r=" << emissive_r << " g=" << emissive_g << " b="<< emissive_b << " a="<< emissive_a << endl;
			if(ambient != NULL)
				cout << "ambient  r=" << ambient_r  << " g=" << ambient_g  << " b="<< ambient_b  << " a="<< ambient_a << endl;
			if(diffuse != NULL)
				cout << "diffuse  r=" << diffuse_r  << " g=" << diffuse_g  << " b="<< diffuse_b  << " a="<< diffuse_a << endl;
			if(specular != NULL)
				cout << "specular r=" << specular_r << " g=" << specular_g << " b="<< specular_b << " a="<< specular_a << endl;
			if(shininess != NULL)
				cout << "shininess value=" << shininess_value << endl;
			if(texture != NULL)
				cout << "texture file=" << texture->ValueTStr().c_str() << " length_s=" << texture_length_s << " length_t=" << texture_length_t << endl;

			cout << endl;
			cin.get();
		}

		node=node->NextSiblingElement();
	}
}
