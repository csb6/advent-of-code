#include <iostream>
#include "../../util/intcode.hpp"
#include <optional>

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
    std::vector<int> program{loadProgram("input.txt")};

    int highestOutput = 0;
    std::array<int,5> phases{5, 6, 7, 8, 9};
    do {
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
    } while(std::next_permutation(phases.begin(), phases.end()));

    std::cout << "Highest output: " << highestOutput << "\n";
    
    return 0;
}
