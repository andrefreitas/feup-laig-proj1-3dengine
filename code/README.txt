===== README =====
	This is a project of development of a simple scene 3D Engine with its own XML Schema (LSF).
	We are using openGL, Glut, Glui, CGFlib from FEUP and TinyXML to parse XML files. The executable
	receives the scene file as argument or uses the default.lsf file.
	
===== AUTHORS =====
	André Freitas - p.andrefreitas@gmail.com
	Paulo Araújo - paulojmaraujo@gmail.com

===== HOW TO COMPILE? =====
	Import the Project in the Eclipse or use the Makefile.

===== WHAT THE PARSER ACCEPTS? =====
	The parser accepts primitives like Triangle, Rectangle, Cylinder, Torus and Sphere. Regarding
	about the Lights, it accepts until 8 Omnilights or Spotlights. About the cameras, accepts Ortho
	and Perspective cameras and accepts materials with textures. We accept rendering configurations 
	like polygon mode and culling that are defined in the Globals section.

===== HOW IS THE RENDER? =====	
	Since the LSF file uses a SceneGraph, the Render module do a DFS in the Graph, associating the 
	correct transformations and textures to the parents and childs in a hierarchy logic.

===== HOW IS ORGANIZED? =====	
	"KEEP IT SIMPLE AND SINCERE", is our philosophy. We use OOP when necessary and simplify the most
	possible when is possible using maps. The project contains three main modules that are LSFparser, 
	LSFobjects and LSFrender. They, respectivly, read, store and render the Scene Objects. We built a 
	FrontEnd with GLUI that is implemented in LSFinterface. All the 3 main modules are used in the LSFscene, 
	that calls them in the 3 different stages (parsing, store, render). As an important auxiliar module, we 
	have built the LSFvertex file, that allows basic and useful operations about vertex, like computation of 
	normals, UV coordinates and etc. Please read carefully the headers first to understand the logic of the project.

===== HOW WE HANDLE ERRORS? =====
	The tinyXML itself implements XML validation of errors. But, we validate logical errors, like more than 8 lights
	defined, reference to nodes that doesn't exist, invalid or missing attributes in LSF tags (ex. missing "x" attribute
	in <scale> tag).

===== IT'S EASY TO ADD PRIMITIVES AND NEW TRANSFORMATIONS? =====
	Yes it is! To add a new primitive just simple change the getNodes int he LSFparser and add a new if clause for the
	primitive name (use the LSFprimitive class), put the new attributes in the map and add another "if" in the Render and it's done.
	About the transformations, in the getNodes add a new "if" and, by the new attributes, call the correct openGL functions inside
	the "if" body. 
	
===== THE OBJECTS APPEAR DARK, WHY? =====
	If the angle of the spotlights isn't enough, the light will not reach the objects. The other cause might be the "emissividade" that is 0.