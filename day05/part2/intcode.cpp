#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>

std::array<int, 4> parse(int instruction)
{
    auto opcode = instruction % 100;
    auto param1Mode = (instruction / 100) % 10;
    auto param2Mode = (instruction / 1000) % 10;
    auto param3Mode = (instruction / 10000) % 10;
    return {opcode, param1Mode, param2Mode, param3Mode};
}

int getValue(const std::vector<int> &program, int mode, int instruction)
{
    if(mode == 0) {
	auto value = program[instruction];
	return value;
    } else if(mode == 1) {
	return instruction;
    } else {
	std::cerr << "Error: unknown mode: " << mode << " for instruction: "
		  << instruction << '\n';
	exit(1);
	return -1;
    }
}

void printParseOutput(int instruction)
{
    auto[opcode, param1Mode, param2Mode, param3Mode] = parse(instruction);
    std::cout << "Instruction: " << instruction << '\n';
    std::cout << "\tOpcode: " << opcode << '\n';
    std::cout << "\tparam1Mode: " << param1Mode << '\n';
    std::cout << "\tparam2Mode: " << param2Mode << '\n';
    std::cout << "\tparam3Mode: " << param3Mode << '\n';
}

void run(std::vector<int> &program)
{
    unsigned int i = 0;
    while(i < program.size()) {
	auto[opcode, param1Mode, param2Mode, param3Mode] = parse(program[i]);
	if(opcode == 1) {
	    // Add
	    auto a = getValue(program, param1Mode, program[i+1]);
	    auto b = getValue(program, param2Mode, program[i+2]);
	    auto destination = program[i+3];
	    program[destination] = a + b;
	    i += 4;
	} else if(opcode == 2) {
	    // Multiply
	    auto a = getValue(program, param1Mode, program[i+1]);
	    auto b = getValue(program, param2Mode, program[i+2]);
	    auto destination = program[i+3];
	    program[destination] = a * b;
	    i += 4;
	} else if(opcode == 3) {
	    // Replace with input
	    auto destination = program[i+1];
	    std::cout << "Enter value: ";
	    int value;
	    std::cin >> value;
	    program[destination] = value;
	    i += 2;
	} else if(opcode == 4) {
	    // Print value
	    auto value = getValue(program, param1Mode, program[i+1]);
	    std::cout << value << '\n';
	    i += 2;
	} else if(opcode == 5) {
	    // Jump if true
	    auto condition = getValue(program, param1Mode, program[i+1]);
	    if(condition != 0) {
		auto value = getValue(program, param2Mode, program[i+2]);
		i = value;
	    } else {
		i += 3;
	    }
	} else if(opcode == 6) {
	    // Jump if false
	    auto condition = getValue(program, param1Mode, program[i+1]);
	    if(condition == 0) {
		auto value = getValue(program, param2Mode, program[i+2]);
		i = value;
	    } else {
		i += 3;
	    }
	} else if(opcode == 7) {
	    // Less than
	    auto a = getValue(program, param1Mode, program[i+1]);
	    auto b = getValue(program, param2Mode, program[i+2]);
	    auto destination = program[i+3];
	    if(a < b) {
		program[destination] = 1;
	    } else {
		program[destination] = 0;
	    }
	    i += 4;
	} else if(opcode == 8) {
	    // Equal to
	    auto a = getValue(program, param1Mode, program[i+1]);
	    auto b = getValue(program, param2Mode, program[i+2]);
	    auto destination = program[i+3];
	    if(a == b) {
		program[destination] = 1;
	    } else {
		program[destination] = 0;
	    }
	    i += 4;
	} else if(opcode == 99) {
	    // Stop program immediately
	    break;
	} else {
	    std::cerr << "Error: unknown opcode: " << opcode
		      << " for the instruction: " << program[i] << " at index "
		      << i << '\n';
	    exit(1);
	}
    }
}

int main()
{
    std::ifstream file("input.txt");
    std::vector<int> program;

    while(file) {
	std::string cell;
	std::getline(file, cell, ',');
	if(cell.size() == 0 || cell == "\n") continue;
	int curr;
	try {
	    curr = std::stoi(cell);
	} catch(const std::invalid_argument &e) {
	    std::cerr << "Error: Can't process: " << cell << '\n';
	    continue;
	}
	// Add each int in the program
	program.push_back(curr);
    }

    run(program);
    
    /*printParseOutput(1002);
    printParseOutput(104);
    printParseOutput(3);*/
    
    return 0;
}
