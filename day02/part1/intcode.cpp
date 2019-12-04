#include <iostream>
#include <fstream>
#include <vector>

int main()
{
    std::ifstream file("input.txt");
    std::vector<unsigned int> program;

    while(file) {
	std::string cell;
	std::getline(file, cell, ',');
	bool success = true;
	for(char digit : cell) {
	    if(digit < '0' || digit > '9') {
		success = false;
		break;
	    }
	}
	if(cell.size() == 0 || !success) continue;
	unsigned int curr;
	try {
	    curr = std::stoi(cell);
	} catch(const std::invalid_argument &e) {
	    std::cerr << "Error: Can't process: " << cell << '\n';
	    continue;
	}
	// Add each int in the program
	program.push_back(curr);
    }
    
    for(unsigned int i = 0; i < program.size(); i += 4) {
	unsigned int num = program[i];
	if(num == 1) {
	    // Add
	    auto a = program[i+1];
	    auto b = program[i+2];
	    auto destination = program.at(i+3);
	    program[destination] = program[a] + program[b];
	} else if(num == 2) {
	    // Multiply
	    auto a = program.at(i+1);
	    auto b = program.at(i+2);
	    auto destination = program.at(i+3);
	    program[destination] = program[a] * program[b];
	} else if(num == 99) {
	    // Stop program immediately
	    break;
	} else {
	    std::cerr << "Error: unknown opcode: " << num << '\n';
	}
    }

    for(auto each : program) { std::cout << each << ','; }
    std::cout << '\n';
    return 0;
}
