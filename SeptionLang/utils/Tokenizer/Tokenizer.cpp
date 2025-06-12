#include <fstream>
#include <string>
#include <iostream>
#include <array>

// custom includes
#include "../Labels.h"
#include "Tokenizer.h"
#include "../ProgramData.h"
#include "../Stack.h"


using namespace std;

#define OPCODE(name) name,
enum class Opcode {
#include "../Opcodes/Opcodes.def"
};
#undef OPCODE

#define OPCODE(name) {#name, Opcode::name},
static std::unordered_map<std::string, Opcode> opcodeMap = {
	#include "../Opcodes/Opcodes.def"
};
#undef OPCODE


int Tokenizer::tokenindex = 0;

void Tokenizer::TokenizeInstruction(string opcode, int lineNumber)
{

	// check if line is empty
	if (opcode.empty()) return;

	// check if opcode is label
	if (opcode.ends_with(":"))
	{
		string labelName = opcode.substr(0, opcode.length() - 1);
		Labels::add(labelName, tokenindex);
		return;
	}


	// extract opcode type
	std::size_t spacePos = opcode.find(' ');
	std::string opType = (spacePos == std::string::npos) ? opcode : opcode.substr(0, spacePos);

	// add opcode to program array
	ProgramData::Program.push_back(opType);
	tokenindex++;
	auto it = opcodeMap.find(opType);
	if (it == opcodeMap.end()) {
		cout << "[Error] Unknown opcode on Line: " << lineNumber << endl;
		exit(EXIT_FAILURE);
	}
	Opcode code = it->second;

	if (code == Opcode::HX) return;
	switch (code)
	{
	case Opcode::PUSH: {
		std::size_t spacePos = opcode.find(' ');
		if (spacePos == std::string::npos || spacePos + 1 >= opcode.size()) {
			std::cout << "[Error] PUSH requires an argument on line: " << lineNumber << std::endl;
			exit(EXIT_FAILURE);
		}
		std::string arg = opcode.substr(spacePos + 1);
		if (arg.empty()) {
			std::cout << "[Error] PUSH argument empty on line: " << lineNumber << std::endl;
			exit(EXIT_FAILURE);
		}

		if (arg.front() == '"' && arg.back() == '"') {
			std::string strValue = arg.substr(1, arg.size() - 2);
			ProgramData::Program.push_back(strValue);
			tokenindex++;
		}
		else {
			try {
				int numValue = std::stoi(arg);
				ProgramData::Program.push_back(std::to_string(numValue));
				tokenindex++;
			}
			catch (...) {
				std::cout << "[Error] PUSH argument is not a valid integer or string literal on line: " << lineNumber << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		break;
	}


	case Opcode::COP: {
		size_t firstQuote = opcode.find('"');
		size_t secondQuote = opcode.find('"', firstQuote + 1);
		size_t thirdQuote = opcode.find('"', secondQuote + 1);

		if (firstQuote == std::string::npos || secondQuote == std::string::npos || thirdQuote != std::string::npos)
		{
			// exit on syntax error
			cout << "[Error] Syntax Error on Line: " << lineNumber << endl;
			exit(EXIT_FAILURE);
		}

		std::string value = opcode.substr(firstQuote + 1, secondQuote - firstQuote - 1);

		ProgramData::Program.push_back(value);
		tokenindex++;
		break;
	}
	case Opcode::JEQ0: {
		std::size_t spacePos = opcode.find(' ');
		if (spacePos != std::string::npos) {
			std::string labelName = opcode.substr(spacePos + 1);
			ProgramData::Program.push_back(labelName);
			tokenindex++;
		}
		break;
	}
	case Opcode::JGT0: {
		std::size_t spacePos = opcode.find(' ');
		if (spacePos == std::string::npos) {
			cout << "[Error] Syntax Error on Line: " << lineNumber << endl;
			exit(EXIT_FAILURE);
		}
		std::string labelName = opcode.substr(spacePos + 1);
		ProgramData::Program.push_back(labelName);
		tokenindex++;
		break;
	}
	case Opcode::JMP: {
		std::size_t spacePos = opcode.find(' ');
		if (spacePos == std::string::npos) {
			std::cout << "[Error] Syntax Error on Line: " << lineNumber << std::endl;
			exit(EXIT_FAILURE);
		}
		std::string labelName = opcode.substr(spacePos + 1);
		ProgramData::Program.push_back(labelName);
		tokenindex++;
		break;
	}


	case Opcode::CALL: {
		std::size_t spacePos = opcode.find(' ');
		if (spacePos != std::string::npos) {
			std::string labelName = opcode.substr(spacePos + 1);
			ProgramData::Program.push_back(labelName);
			tokenindex++;
		}
		else {
			// exit on syntax error
			cout << "[Error] Syntax Error on Line: " << lineNumber << endl;
			exit(EXIT_FAILURE);
		}
		break;
	}


	}
}