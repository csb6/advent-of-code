#include <iostream>
#include "../../util/intcode.hpp"

int run(std::vector<int> &program, std::array<int,2> inputs)
{
    unsigned int i = 0;
    int currInput = 0;
    int lastPrintedValue = 0;
    while(i < program.size()) {
	assert(currInput <= 2);
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
	    //std::cout << "Enter value: " << inputs.at(currInput) << '\n';
	    //std::cin >> value;
	    program[destination] = inputs.at(currInput);
	    ++currInput;
	    i += 2;
	} else if(opcode == 4) {
	    // Print value
	    auto value = getValue(program, param1Mode, program[i+1]);
	    //std::cout << value << '\n';
	    lastPrintedValue = value;
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
    return lastPrintedValue;
}

int main()
{
    std::vector<int> program{loadProgram("input.txt")};

    int highestOutput = 0;
    std::array<int,5> inputs{0, 1, 2, 3, 4};
    do {
	std::vector<int> progA{program};
	auto outputA = run(progA, {inputs[0], 0});

	std::vector<int> progB{program};
	auto outputB = run(progB, {inputs[1], outputA});

	std::vector<int> progC{program};
	auto outputC = run(progC, {inputs[2], outputB});

	std::vector<int> progD{program};
	auto outputD = run(progD, {inputs[3], outputC});

	std::vector<int> progE{program};
	auto outputE = run(progE, {inputs[4], outputD});
	if(outputE > highestOutput) {
	    highestOutput = outputE;
	}
    } while(std::next_permutation(inputs.begin(), inputs.end()));

    std::cout << "Highest output: " << highestOutput << "\n";
    
    return 0;
}
