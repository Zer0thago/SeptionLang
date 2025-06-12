#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <array>

using namespace std;

class Tokenizer {
public:
	static void TokenizeInstruction(string opcode, int lineNumber);
private:
	static int tokenindex;
};