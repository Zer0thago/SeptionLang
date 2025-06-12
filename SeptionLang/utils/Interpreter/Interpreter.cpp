#include <fstream>
#include <string>
#include <iostream>
#include <array>
#include <algorithm> 

// custom includes
#include "../Labels.h"
#include "Interpreter.h"
#include "../ProgramData.h"
#include "../Stack.h"
#include "../Tokenizer/Tokenizer.h"

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


// set up global variables
std::vector<std::string> ProgramData::Program;
std::vector<int> ProgramData::ReturnStack;

using Value = std::variant<int, std::string>;

int getInt(const Value& v) {
	if (std::holds_alternative<int>(v)) return std::get<int>(v);
	throw std::runtime_error("Expected int on stack");
}

std::string getString(const Value& v) {
	if (std::holds_alternative<std::string>(v)) return std::get<std::string>(v);
	throw std::runtime_error("Expected string on stack");
}

[[noreturn]] void debuggerError(int line, const string& message) {
	cerr << "program.sl:" << line << ": error: " << message << endl;
	exit(EXIT_FAILURE);
}

void Interpreter::run(char* path) {
	srand(time(0));

	std::ifstream file(path);
	std::string line;

	int line_number = 1;
	while (std::getline(file, line)) {
		if (line.empty() || line[0] == ';') {
			line_number++;
			continue;
		}

		Tokenizer::TokenizeInstruction(line, line_number);
		line_number++;
	}

	InterpretInstruction();
}

void Interpreter::InterpretInstruction()
{
	int pc = 0;

	Stack stack = Stack();

	while (true)
	{
		if (pc >= ProgramData::Program.size()) {
			std::cerr << "Program counter out of range: " << pc << std::endl;
			exit(EXIT_FAILURE);
		}
		std::string instruction = ProgramData::Program[pc];
		pc++;

		switch (opcodeMap[instruction])
		{
			case Opcode::PUSH: {
				// push a
				std::string val = ProgramData::Program[pc];
				pc++;
				try {
					int num = std::stoi(val);
					stack.push(num);
				}
				catch (...) {
					stack.push(val);
				}
				break;
			}

			case Opcode::HX: {
				
				// exit
				exit(EXIT_SUCCESS);
			}
			case Opcode::COP: {
				// Print a fixed string/int
				string value = ProgramData::Program[pc];
				pc++;

				cout << value;
				break;
			}
			case Opcode::NL: {
				// new line
				cout << endl;
				break;
			}
			case Opcode::ADD: {
				// add a b
				auto a = stack.pop();
				auto b = stack.pop();

				if (!std::holds_alternative<int>(a) || !std::holds_alternative<int>(b)) {
					std::cout << "ADD expects two integers on the stack\n";
					exit(EXIT_FAILURE);
				}

				int result = std::get<int>(a) + std::get<int>(b);
				stack.push(result);
				break;
			}

			case Opcode::SUB: {
				// sub a b
				auto a = stack.pop();
				auto b = stack.pop();

				if (!std::holds_alternative<int>(a) || !std::holds_alternative<int>(b)) {
					std::cout << "SUB expects two integers on the stack\n";
					exit(EXIT_FAILURE);
				}

				int result = std::get<int>(b) - std::get<int>(a);
				stack.push(result);
				break;
			}
			case Opcode::DIV: {
				// div a b 
				auto a = stack.pop();
				auto b = stack.pop();

				if (!std::holds_alternative<int>(a) || !std::holds_alternative<int>(b)) {
					std::cout << "DIV expects two integers on the stack\n";
					exit(EXIT_FAILURE);
				}

				int divisor = std::get<int>(a);
				if (divisor == 0) {
					std::cout << "Division by zero error\n";
					exit(EXIT_FAILURE);
				}

				int result = std::get<int>(b) / divisor;
				stack.push(result);
				break;
			}
			case Opcode::MOD: {
				// mod a b 
				auto a = stack.pop();
				auto b = stack.pop();

				if (!std::holds_alternative<int>(a) || !std::holds_alternative<int>(b)) {
					std::cout << "MOD expects two integers on the stack\n";
					exit(EXIT_FAILURE);
				}

				int divisor = std::get<int>(a);
				if (divisor == 0) {
					std::cout << "Modulo by zero error\n";
					exit(EXIT_FAILURE);
				}

				int result = std::get<int>(b) % divisor;
				stack.push(result);
				break;
			}

			case Opcode::HLT: {
				// halt
				return;
			}
			case Opcode::PRINT: {
				// print top of stack to stdout
				auto val = stack.pop();
				std::visit([](auto&& arg) {
					std::cout << arg;
					}, val);
				break;
			}
			case Opcode::DP: {
				// duplicate top of stack
				auto val = stack.top();
				stack.push(val);
				break;
			}

			case Opcode::JEQ0: {
				// jump if equal to 0
				auto val = stack.pop();
				string label = ProgramData::Program[pc];
				pc++;
				int addr = Labels::get(label);
				if (addr == -1) {
					cout << "Label not found: " << label << endl;
					exit(EXIT_FAILURE);
				}

				bool isZero = std::visit([](auto&& arg) -> bool {
					using T = std::decay_t<decltype(arg)>;
					if constexpr (std::is_same_v<T, int>)
						return arg == 0;
					else
						return false; 
					}, val);

				if (isZero) {
					pc = addr;
				}
				break;
			}

			case Opcode::JGT0: {
				// jump if greater than 0
				auto val = stack.pop();
				string label = ProgramData::Program[pc];
				pc++;
				int addr = Labels::get(label);
				if (addr == -1) {
					cout << "Label not found: " << label << endl;
					exit(EXIT_FAILURE);
				}

				bool isGreaterThanZero = std::visit([](auto&& arg) -> bool {
					using T = std::decay_t<decltype(arg)>;
					if constexpr (std::is_same_v<T, int>)
						return arg > 0;
					else
						return false; 
					}, val);

				if (isGreaterThanZero) {
					pc = addr;
				}
				break;
			}

			case Opcode::CIP: {
				// read from stdin and push to stack
				try
				{
					std::string input;
					std::getline(std::cin, input);

					auto isNumber = [](const std::string& s) {
						if (s.empty()) return false;
						size_t start = 0;
						if (s[0] == '-') start = 1;
						return std::all_of(s.begin() + start, s.end(), ::isdigit);
						};

					if (isNumber(input)) {
						stack.push(std::stoi(input));
					}
					else {
						stack.push(input);
					}
					break;
				}
				catch (const std::exception& e)
				{
					cout << e.what() << endl;
				}
				break;

			}
			case Opcode::CALL:
			{
				// call a label and push the return address
				std::string label = ProgramData::Program[pc++];

				ProgramData::ReturnStack.push_back(pc);

				int addr = Labels::get(label);
				if (addr == -1) {
					debuggerError(pc - 1, "undefined label: " + label);
				}

				pc = addr;
				break;
			}
			case Opcode::SWP: {
				// swap the top two elements
				auto a = stack.pop();
				auto b = stack.pop();
				stack.push(a); 
				stack.push(b); 
				break;
			}

			case Opcode::POP: {
				// remove the top element
				stack.pop(); 
				break;
			}
			case Opcode::MUL: {
				// multiply the top two elements
				auto a = stack.pop();
				auto b = stack.pop();

				if (!std::holds_alternative<int>(a) || !std::holds_alternative<int>(b)) {
					std::cout << "MUL expects two integers on the stack\n";
					exit(EXIT_FAILURE);
				}

				int result = std::get<int>(a) * std::get<int>(b);
				stack.push(result);
				break;
			}

			case Opcode::RET:
			{
				// return from a function
				if (ProgramData::ReturnStack.empty()) {
					debuggerError(pc - 1, "RET with empty return stack");
				}

				int returnAddr = ProgramData::ReturnStack.back();
				ProgramData::ReturnStack.pop_back();

				pc = returnAddr;
				break;
			}

			case Opcode::JMP: {
				// jump to a label
				std::string label = ProgramData::Program[pc++];
				int addr = Labels::get(label);
				if (addr == -1) {
					debuggerError(pc - 1, "undefined label: " + label);
				}
				pc = addr;
				break;
			}
			case Opcode::RND: {
				// random number between min and max
				int max = getInt(stack.pop());
				int min = getInt(stack.pop());
				int range = max - min + 1;
				int random_num = min + (rand() % range);

				stack.push(random_num);
				break;
			}

			default: {
				std::cout << "Unknown opcode: " << ProgramData::Program[pc] << std::endl;
				exit(EXIT_FAILURE);
			}

		}
	}
}
