#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>
#include <optional>

std::array<int, 4> parse(int instruction)
{
    auto opcode = instruction % 100;
    auto param1Mode = (instruction / 100) % 10;
    auto param2Mode = (instruction / 1000) % 10;
    auto param3Mode = (instruction / 10000) % 10;
    return {opcode, param1Mode, param2Mode, param3Mode};
}

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

class Amp {
private:
    int i = 0;
    bool needsFirstInput = true;
    int m_phase;
    std::vector<int> m_program;
public:
    Amp(std::vector<int> &program, int phase)
	: m_phase{phase}, m_program{program}
    {}
    
    std::optional<int> operator()(int input)
    {
	while(i < m_program.size()) {
	    auto[opcode, param1Mode, param2Mode, param3Mode] = parse(m_program[i]);
	    if(opcode == 1) {
		// Add
		auto a = getValue(m_program, param1Mode, m_program[i+1]);
		auto b = getValue(m_program, param2Mode, m_program[i+2]);
		auto destination = m_program[i+3];
		m_program[destination] = a + b;
		i += 4;
	    } else if(opcode == 2) {
		// Multiply
		auto a = getValue(m_program, param1Mode, m_program[i+1]);
		auto b = getValue(m_program, param2Mode, m_program[i+2]);
		auto destination = m_program[i+3];
		m_program[destination] = a * b;
		i += 4;
	    } else if(opcode == 3) {
		// Replace with input
		auto destination = m_program[i+1];
		//std::cout << "Enter value: " << inputs.at(currInput) << '\n';
		//std::cin >> value;
		if(!needsFirstInput) {
		    m_program[destination] = input;
		} else {
		    m_program[destination] = m_phase;
		    needsFirstInput = false;
		} 
		i += 2;
	    } else if(opcode == 4) {
		// Print value
		auto value = getValue(m_program, param1Mode, m_program[i+1]);
		//std::cout << value << '\n';
		i += 2;
		return {value};
	    } else if(opcode == 5) {
		// Jump if true
		auto condition = getValue(m_program, param1Mode, m_program[i+1]);
		if(condition != 0) {
		    auto value = getValue(m_program, param2Mode, m_program[i+2]);
		    i = value;
		} else {
		    i += 3;
		}
	    } else if(opcode == 6) {
		// Jump if false
		auto condition = getValue(m_program, param1Mode, m_program[i+1]);
		if(condition == 0) {
		    auto value = getValue(m_program, param2Mode, m_program[i+2]);
		    i = value;
		} else {
		    i += 3;
		}
	    } else if(opcode == 7) {
		// Less than
		auto a = getValue(m_program, param1Mode, m_program[i+1]);
		auto b = getValue(m_program, param2Mode, m_program[i+2]);
		auto destination = m_program[i+3];
		if(a < b) {
		    m_program[destination] = 1;
		} else {
		    m_program[destination] = 0;
		}
		i += 4;
	    } else if(opcode == 8) {
		// Equal to
		auto a = getValue(m_program, param1Mode, m_program[i+1]);
		auto b = getValue(m_program, param2Mode, m_program[i+2]);
		auto destination = m_program[i+3];
		if(a == b) {
		    m_program[destination] = 1;
		} else {
		    m_program[destination] = 0;
		}
		i += 4;
	    } else if(opcode == 99) {
		// Stop m_program immediately
		return {};
	    } else {
		std::cerr << "Error: unknown opcode: " << opcode
			  << " for the instruction: " << m_program[i] << " at index "
			  << i << '\n';
		exit(1);
	    }
	}
	return {};
    }
};

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

    int highestOutput = 0;
    std::array<int,5> phases{5, 6, 7, 8, 9};
    while(std::next_permutation(phases.begin(), phases.end())) {
	Amp a(program, phases[0]);
	Amp b(program, phases[1]);
	Amp c(program, phases[2]);
	Amp d(program, phases[3]);
	Amp e(program, phases[4]);
	int outputE = 0;
	while(true) {
	    int outputA = a(outputE).value_or(0);
	    int outputB = b(outputA).value_or(0);
	    int outputC = c(outputB).value_or(0);
	    int outputD = d(outputC).value_or(0);
	    auto result = e(outputD);
	    if(!result) {
		break;
	    } else {
		outputE = *result;
	    }
	}
	if(outputE > highestOutput) {
	    highestOutput = outputE;
	}
    }

    std::cout << "Highest output: " << highestOutput << "\n";
    
    return 0;
}
