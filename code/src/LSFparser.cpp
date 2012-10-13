#include <iostream>
#include "LSFParser.h"
#include "LSFobjects.h"
#include <map>
#include <stack>
#include <CGFapplication.h>
#include <vector>
#include "CGFlight.h"
#include "LSFvertex.h"
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
		lsfElement = lsfFile->FirstChildElement( "lsf" );
		globalsElement = lsfElement->FirstChildElement( "globals" );
		camerasElement = lsfElement->FirstChildElement( "cameras" );
		graphElement = lsfElement->FirstChildElement( "graph" );
		appearancesElement = lsfElement->FirstChildElement( "appearances" );
		lightingsElement = lsfElement->FirstChildElement( "lighting" );
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

void LSFparser::getCameras(map<string, LSFcamera*> &cameras){
	TiXmlElement *node=camerasElement->FirstChildElement();
	int counter=0;
	const char *initial;
	initial=camerasElement->Attribute("initial");
	if(DEBUGMODE) cout << "\n--- Cameras: " << initial << " ---";
	// Loop trough cameras
	while(node){
		counter++;

		LSFcamera *camera = new LSFcamera();
		camera->view.assign(node->Value());

		// -->
		if(strcmp(node->Value(), "ortho")==0){
			camera->id=node->Attribute("id");
			node->QueryFloatAttribute("near",&camera->_near);
			node->QueryFloatAttribute("far",&camera->_far);
			node->QueryFloatAttribute("left",&camera->left);
			node->QueryFloatAttribute("right",&camera->right);
			node->QueryFloatAttribute("top",&camera->top);
			node->QueryFloatAttribute("bottom",&camera->bottom);

			if(DEBUGMODE){
				float attributes[]={camera->_near,camera->_far,camera->left,camera->right,camera->top,camera->bottom};
				cout << "\n\tOrtho: " << camera->id << " ";
				for(int unsigned i=0; i<6; i++) cout << attributes[i] << " ";
				cout << endl;
			}
		}
		else{
			camera->id=node->Attribute("id");
			node->QueryFloatAttribute("near",&camera->_near);
			node->QueryFloatAttribute("far",&camera->_far);
			node->QueryFloatAttribute("angle",&camera->angle);

			// From
			TiXmlElement *from, *to;
			from=node->FirstChildElement("from");
			from->QueryFloatAttribute("x",&camera->fromX);
			from->QueryFloatAttribute("y",&camera->fromY);
			from->QueryFloatAttribute("z",&camera->fromZ);

			// To
			to=node->FirstChildElement("to");
			to->QueryFloatAttribute("x",&camera->toX);
			to->QueryFloatAttribute("y",&camera->toY);
			to->QueryFloatAttribute("z",&camera->toZ);

			if(DEBUGMODE){
				float attributes[]={camera->_near,camera->_far,camera->angle};
				cout << "\n\tPerspective: " << camera->id << " ";
				for(int unsigned i=0; i<3; i++) cout << attributes[i] << " ";
				cout << endl;
				cout << "\t\t\tFrom: " << camera->fromX << " " << camera->fromY << " " << camera->fromZ << endl;
				cout << "\t\t\tTo: " << camera->toX << " " << camera->toY << " " << camera->toZ << endl;

			}

		}
		if(camera->id == initial)
			camera->initial = true;
		else
			camera->initial = false;

		cameras[camera->id] = camera;

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
				if(axis=='x') x=1;
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
		pnode->appearance.assign(appearance);
		if(DEBUGMODE) cout << "\tAppearance: " << pnode->appearance << endl;


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
				// Compute the normal
				LSFvertex v1(prim.attr["x1"],prim.attr["y1"],prim.attr["z1"]);
				LSFvertex v2(prim.attr["x2"],prim.attr["y2"],prim.attr["z2"]);
				LSFvertex v3(prim.attr["x3"],prim.attr["y3"],prim.attr["z3"]);
				vector<LSFvertex> vertexs;
				vertexs.push_back(v3); // Need to be in this order
				vertexs.push_back(v2);
				vertexs.push_back(v1);

				prim.normal=computeNormalNewel(vertexs);

				// UV coords
				vector<LSFvertex> uvCoords;
				uvCoords.push_back(LSFvertex(prim.attr["x1"],prim.attr["y1"],prim.attr["z1"]));
				uvCoords.push_back(LSFvertex(prim.attr["x2"],prim.attr["y2"],prim.attr["z2"]));
				uvCoords.push_back(LSFvertex(prim.attr["x3"],prim.attr["y3"],prim.attr["z3"]));
				uvCoords=computeTriangleUV(uvCoords);
				prim.uvCoords=uvCoords;
				// -->
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

void LSFparser::getAppearances(map<string,LSFappearance*> &appearances){
	TiXmlElement *node=appearancesElement->FirstChildElement();
	int counter = 0;
	float emissive_vec[4], ambient_vec[4], diffuse_vec[4], specular_vec[4];
	float shininess_value, texture_length_s, texture_length_t;
	cout << "\n--- Aparencias  ---" << endl;
	// Loop trough appearances
	while(node){
		counter++;

		TiXmlElement *emissive, *ambient, *diffuse, *specular, *shininess, *texture;

		emissive = node->FirstChildElement("emissive");
		if(emissive != NULL)
			if(strcmp(emissive->ValueTStr().c_str(),"emissive")==0){
				emissive->QueryFloatAttribute("r",&emissive_vec[0]);
				emissive->QueryFloatAttribute("g",&emissive_vec[1]);
				emissive->QueryFloatAttribute("b",&emissive_vec[2]);
				emissive->QueryFloatAttribute("a",&emissive_vec[3]);
			}

		ambient = node->FirstChildElement("ambient");
		if(ambient != NULL)
			if(strcmp(ambient->ValueTStr().c_str(), "ambient")==0){
				ambient->QueryFloatAttribute("r",&ambient_vec[0]);
				ambient->QueryFloatAttribute("g",&ambient_vec[1]);
				ambient->QueryFloatAttribute("b",&ambient_vec[2]);
				ambient->QueryFloatAttribute("a",&ambient_vec[3]);
			}

		diffuse = node->FirstChildElement("diffuse");
		if(diffuse != NULL)
			if(strcmp(diffuse->ValueTStr().c_str(), "diffuse")==0){
				diffuse->QueryFloatAttribute("r",&diffuse_vec[0]);
				diffuse->QueryFloatAttribute("g",&diffuse_vec[1]);
				diffuse->QueryFloatAttribute("b",&diffuse_vec[2]);
				diffuse->QueryFloatAttribute("a",&diffuse_vec[3]);
			}

		specular = node->FirstChildElement("specular");
		if(specular != NULL)
			if(strcmp(specular->ValueTStr().c_str(), "specular")==0){
				specular->QueryFloatAttribute("r",&specular_vec[0]);
				specular->QueryFloatAttribute("g",&specular_vec[1]);
				specular->QueryFloatAttribute("b",&specular_vec[2]);
				specular->QueryFloatAttribute("a",&specular_vec[3]);
			}

		shininess = node->FirstChildElement("shininess");
		if(shininess != NULL)
			if(strcmp(shininess->ValueTStr().c_str(), "shininess")==0){
				shininess->QueryFloatAttribute("value", &shininess_value);
			}


		texture = node->FirstChildElement("texture");
		if(texture != NULL)
			if(strcmp(texture->ValueTStr().c_str(), "texture")==0){
				texture->QueryFloatAttribute("length_s", &texture_length_s);
				texture->QueryFloatAttribute("length_t", &texture_length_t);
			}

		if(DEBUGMODE){
			cout << "\n\tAparencia: " << node->Attribute("id") << " " << endl;
			if(emissive != NULL){ cout << "emissive "; for(int i=0; i < 4; i++) cout << emissive_vec[i] << " "; cout << endl;}
			if(ambient != NULL) {cout << "ambient "; for(int i=0; i < 4; i++) cout << ambient_vec[i] << " "; cout << endl;}
			if(diffuse != NULL) {cout << "diffuse "; for(int i=0; i < 4; i++) cout << diffuse_vec[i] << " "; cout << endl;}
			if(specular != NULL){ cout << "specular "; for(int i=0; i < 4; i++) cout  << specular_vec[i] << " "; cout << endl;}
			if(shininess != NULL) cout << "shininess value=" << shininess_value << endl;
			if(texture != NULL)
				cout << "texture file=" << texture->Attribute("file") << " length_s=" << texture_length_s << " length_t=" << texture_length_t << endl;

			cout << endl;
		}
		// Add to the map
		CGFappearance* pappearance;
		LSFappearance *pLSFappearance=new LSFappearance();

		if(emissive != NULL)
			pappearance = new CGFappearance(emissive_vec);
		else
			pappearance= new CGFappearance();

		pappearance->setAmbient( ambient_vec);
		pappearance->setDiffuse(diffuse_vec);
		pappearance->setSpecular(specular_vec);
		pappearance->setShininess(shininess_value);
		if(texture != NULL){
			string textureFile;
			textureFile.assign(texture->Attribute("file"));
			CGFtexture *text = new CGFtexture(texture->Attribute("file"));
			text->setSize(texture_length_s, texture_length_t);
			pappearance->setTexture(text);
			pappearance->setTextureWrap(GL_REPEAT,GL_REPEAT);
			if(DEBUGMODE) cout << "\nSettexture: " << textureFile << endl;
		}
		// -->
		string auxId;
		auxId.assign(node->Attribute("id"));
		// Attribute to LSFAppearance
		pLSFappearance->appearance=pappearance;
		pLSFappearance->emissive[0]=emissive_vec[0];
		pLSFappearance->emissive[1]=emissive_vec[1];
		pLSFappearance->emissive[2]=emissive_vec[2];
		pLSFappearance->emissive[3]=emissive_vec[3];
		pLSFappearance->id=auxId;
		if(texture!=NULL){
			pLSFappearance->length_s=texture_length_s;
			pLSFappearance->length_t=texture_length_t;
			pLSFappearance->haveTexture=true;
		}
		else {
			pLSFappearance->haveTexture=false;
		}
		appearances[auxId]=pLSFappearance;
		node=node->NextSiblingElement();
	}

}

void LSFparser::getLights(map<string,LSFlight*>&lights, bool &enabled, bool &local, bool &doublesided, float *sceneAmbient){
	int counter = 0;
	const char *light_id;
	int lightsId[]={GL_LIGHT0,GL_LIGHT1,GL_LIGHT2,GL_LIGHT3,GL_LIGHT4,GL_LIGHT5,GL_LIGHT6,GL_LIGHT7};
	lightingsElement->QueryBoolAttribute("local",&local);
	lightingsElement->QueryBoolAttribute("enabled",&enabled);
	lightingsElement->QueryBoolAttribute("doublesided",&doublesided);
	if(DEBUGMODE){
		cout << "\n--- Luzes  ---" << endl;
		if(doublesided) cout << "doubleSided = true" << endl;
		if(local) cout << "local = true" << endl;
		if(enabled) cout << "enabled = true" << endl;

	}

	TiXmlElement *scene_ambient=lightingsElement->FirstChildElement("ambient");

	//ambiente global
	if(scene_ambient != NULL){
		scene_ambient->QueryFloatAttribute("r",&sceneAmbient[0]);
		scene_ambient->QueryFloatAttribute("g",&sceneAmbient[1]);
		scene_ambient->QueryFloatAttribute("b",&sceneAmbient[2]);
		scene_ambient->QueryFloatAttribute("a",&sceneAmbient[3]);
	}

	if(DEBUGMODE && scene_ambient != NULL)
		cout << "scene ambient r=" << sceneAmbient[0] << " g=" << sceneAmbient[1] << " b=" << sceneAmbient[2] << " a=" << sceneAmbient[3] << endl;

	TiXmlElement *node_=lightingsElement->FirstChildElement("lights");

	TiXmlElement *node = node_->FirstChildElement("light");

	// Loop lights
	while(node){
		CGFlight *plight;
		counter++;
		// Detect if there are more than 8 lights
		if(counter>8){
			cout << "\n\n\n\n ERROR: YOU HAVE MORE THAN 8 LIGHTS! Press to exit \n\n\n";
			getchar(); // wait feedback
			exit(-1);
		}

		// new light
		LSFlight *pLSFlight=new LSFlight;

		float * positionV = new float[4];
		float * directionV= new float[3];
		float * ambientV  = new float[4]; //ambiente de cada luz
		float * diffuseV  = new float[4];
		float * specularV = new float[4];

		float spot_angle, spot_exponent;

		light_id = node->Attribute("id");
		pLSFlight->id=light_id;

		node->QueryBoolAttribute("enabled",&pLSFlight->enabled);

		TiXmlElement *location, *ambient, *diffuse, *specular, *spot;

		location = node->FirstChildElement("location");
		if(location != NULL)
			if(strcmp(location->ValueTStr().c_str(),"location")==0){
				location->QueryFloatAttribute("x",&positionV[0]);
				location->QueryFloatAttribute("y",&positionV[1]);
				location->QueryFloatAttribute("z",&positionV[2]);
				positionV[3] = 1;
			}

		ambient = node->FirstChildElement("ambient");
		if(ambient != NULL)
			if(strcmp(ambient->ValueTStr().c_str(),"ambient")==0){
				ambient->QueryFloatAttribute("r",&ambientV[0]);
				ambient->QueryFloatAttribute("g",&ambientV[1]);
				ambient->QueryFloatAttribute("b",&ambientV[2]);
				ambient->QueryFloatAttribute("a",&ambientV[3]);
			}

		diffuse = node->FirstChildElement("diffuse");
		if(diffuse != NULL)
			if(strcmp(diffuse->ValueTStr().c_str(),"diffuse")==0){
				diffuse->QueryFloatAttribute("r",&diffuseV[0]);
				diffuse->QueryFloatAttribute("g",&diffuseV[1]);
				diffuse->QueryFloatAttribute("b",&diffuseV[2]);
				diffuse->QueryFloatAttribute("a",&diffuseV[3]);
			}

		specular = node->FirstChildElement("specular");
		if(specular != NULL)
			if(strcmp(specular->ValueTStr().c_str(),"specular")==0){
				specular->QueryFloatAttribute("r",&specularV[0]);
				specular->QueryFloatAttribute("g",&specularV[1]);
				specular->QueryFloatAttribute("b",&specularV[2]);
				specular->QueryFloatAttribute("a",&specularV[3]);
			}

		spot= node->FirstChildElement("spot");
		if(spot != NULL){
			if(strcmp(spot->ValueTStr().c_str(),"spot")==0){
				spot->QueryFloatAttribute("angle",&spot_angle);
				spot->QueryFloatAttribute("exponent",&spot_exponent);
				spot->QueryFloatAttribute("dirx",&directionV[0]);
				spot->QueryFloatAttribute("diry",&directionV[1]);
				spot->QueryFloatAttribute("dirz",&directionV[2]);
			}
			pLSFlight->isspotLight = true;
		}
		else{
			directionV = NULL;
			pLSFlight->isspotLight = false;
		}

		// Add light to the vector
		// Create
		plight= new CGFlight(lightsId[counter-1],positionV);
		// Angle for spotlight
		if(spot != NULL) plight->setAngle(spot_angle);
		// Ambient
		plight->setAmbient(ambientV);
		// Diffuse
		plight->setDiffuse(diffuseV);
		// Specular
		plight->setSpecular(specularV);

		if(spot != NULL) {
			glLightf( lightsId[counter-1], GL_SPOT_EXPONENT, spot_exponent);
			pLSFlight->spotExponent= spot_exponent;
			glLightf(lightsId[counter-1], GL_SPOT_CUTOFF, spot_angle);
			glLightfv(lightsId[counter-1], GL_SPOT_DIRECTION, directionV);
		}
		else{
			pLSFlight->spotExponent= -1; // Maybe usefull later
		}

		pLSFlight->light=plight;

		lights[light_id]=pLSFlight;

		if(DEBUGMODE){
			cout << "\n\tLuz: " << light_id << "  enabled=" << pLSFlight->enabled << endl;
			if(location != NULL)
				cout << "location x=" << positionV[0] << " y=" << positionV[1] << " z="<< positionV[2] << endl;
			if(ambient != NULL)
				cout << "ambient  r=" << ambientV[0]  << " g=" << ambientV[1] << " b="<< ambientV[2]  << " a="<< ambientV[3] << endl;
			if(diffuse != NULL)
				cout << "diffuse  r=" << diffuseV[0]  << " g=" << diffuseV[1]  << " b="<< diffuseV[2]  << " a="<< diffuseV[3] << endl;
			if(specular != NULL)
				cout << "specular r=" << specularV[0] << " g=" << specularV[1] << " b="<< specularV[2] << " a="<< specularV[4] << endl;
			if(spot != NULL){
				cout << "spot angle=" << spot_angle << " exponent=" << spot_exponent;
			    cout << " dirx=" << directionV[0] << " diry=" << directionV[1] << " dirz=" << directionV[2] << endl;
			}
			cout << endl;
		}

		node=node->NextSiblingElement();
	}
}
