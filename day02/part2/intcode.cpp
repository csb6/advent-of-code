#include <iostream>
#include <fstream>
#include <vector>

void run(std::vector<unsigned int> &program)
{
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
}

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

    int sum = 0;
    int noun = 0;
    int verb = 0;
    while(sum != 19690720 && noun <= 99) {
	verb = 0;
	while(sum != 19690720 && verb <= 99) {
	    std::vector<unsigned int> copy{program};
	    copy[1] = noun;
	    copy[2] = verb;
	    run(copy);
	    sum = copy[0];
	    ++verb;
	}
	++noun;
    }
    std::cout << "Noun: " << noun-1 << " " << "Verb: " << verb-1 << " Sum: " << sum << '\n';
    std::cout << "Answer: " << 100 * (noun-1) + (verb-1) << '\n';
    
    //for(auto each : program) { std::cout << each << ','; }
    std::cout << '\n';
    return 0;
}
