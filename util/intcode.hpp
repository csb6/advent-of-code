#include <vector>
#include <string>
#include <array>
#include <vector>
#include <fstream>

// Reads IntCode program in from a CSV file; make sure that the
// file has a trailing ',', or last element will be skipped
std::vector<int> loadProgram(const char *path)
{
    std::ifstream file(path);
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
    return program;
}

// Separate an intcode instruction into the opcode/params it represents
std::array<int, 4> parse(int instruction)
{
    auto opcode = instruction % 100;
    auto param1Mode = (instruction / 100) % 10;
    auto param2Mode = (instruction / 1000) % 10;
    auto param3Mode = (instruction / 10000) % 10;
    return {opcode, param1Mode, param2Mode, param3Mode};
}

// Returns the value of a given parameter based on its mode
// Positional mode: use the parameter as a memory address, return
//     the address' value
// Immediate mode: use the parameter itself as the value
int getValue(const std::vector<int> &program, int mode, int param)
{
    if(mode == 0) {
	// Positional mode
	auto value = program[param];
	return value;
    } else if(mode == 1) {
	// Immediate mode
	return param;
    } else {
	std::cerr << "Error: unknown mode: " << mode << " for instruction: "
		  << param << '\n';
	exit(1);
    }
}
