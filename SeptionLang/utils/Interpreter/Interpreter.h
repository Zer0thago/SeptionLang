#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <array>

using namespace std;

class Interpreter {
public:
	void run(char* path);
private:
	void InterpretInstruction();
};