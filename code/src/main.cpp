#include <iostream>
#include <exception>
#include "CGFapplication.h"
#include "LSFscene.h"
#include "LSFinterface.h"
#include "LSFnode.h"
using std::cout;
using std::exception;
using namespace std;
int main(int argc, char* argv[]) {

	CGFapplication app = CGFapplication();

	try {
		app.init(&argc, argv);

		app.setScene(new LSFscene(argv));
		app.setInterface(new LSFinterface());
		
		app.run();
	}
	catch(GLexception& ex) {
		cout << "Erro: " << ex.what();
		return -1;
	}
	catch(exception& ex) {
		cout << "Erro inesperado: " << ex.what();
		return -1;
	}
	return 0;
}
