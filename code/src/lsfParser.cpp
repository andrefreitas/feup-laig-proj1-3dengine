#include <iostream>

#include "lsfParser.h"


lsfParser::lsfParser(char* inputFile){
	file = new TiXmlDocument(inputFile);

	if(file->LoadFile()){
		std::cout << "File ring.lsf is load" << std::endl;
	}
	else{
		std::cout << "Error openning file ring.lsf" << std::endl;
	}
}
