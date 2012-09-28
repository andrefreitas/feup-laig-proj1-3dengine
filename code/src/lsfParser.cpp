#include <iostream>

#include "lsfParser.h"


lsfParser::lsfParser(char* inputFile){
	file = new TiXmlDocument(inputFile);

	if(file->LoadFile()){
		std::cout << "File " << inputFile << " is load" << std::endl;
	}
	else{
		std::cout << "Error openning file " << inputFile << std::endl;
	}
}
