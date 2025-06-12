#include <iostream>
#include <fstream>
#include <string>
#include "utils/Interpreter/Interpreter.h"

using namespace std;

int main(int argc, char* argv[])
{
	try
	{
		// Hellu this is SeptionLang :D

		// Get the path of the file that we want to interpret
		char* path = argv[1];
		if (path == NULL)
		{
			cout << "Please select a .sl file" << endl;
			return 0;
		}

		// Pass the files to the interpreter
		Interpreter interpreter = Interpreter();
		interpreter.run(path);
	}
	catch (std::exception& e)
	{
		cout << e.what() << endl;
	}


	return 0;
}