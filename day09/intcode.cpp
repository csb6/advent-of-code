// To see Part 1 output: enter '1'
// To see Part 2 output: enter '2'
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>
#include <type_traits>
#include <optional>

std::array<long long, 4> parse(int instruction)
{
    auto opcode = instruction % 100;
    auto param1Mode = (instruction / 100) % 10;
    auto param2Mode = (instruction / 1000) % 10;
    auto param3Mode = (instruction / 10000) % 10;
    return {opcode, param1Mode, param2Mode, param3Mode};
}

void printParseOutput(long long instruction)
{
    auto[opcode, param1Mode, param2Mode, param3Mode] = parse(instruction);
    std::cout << "Instruction: " << instruction << '\n';
    std::cout << "\tOpcode: " << opcode << '\n';
    std::cout << "\tparam1Mode: " << param1Mode << '\n';
    std::cout << "\tparam2Mode: " << param2Mode << '\n';
    std::cout << "\tparam3Mode: " << param3Mode << '\n';
}

auto loadProgram(const char *path)
{
    std::ifstream file(path);
    std::vector<long long> program;
    program.resize(program.size() * 10);

    while(file) {
	std::string cell;
	std::getline(file, cell, ',');
	if(cell.size() == 0 || cell == "\n") continue;
	long long curr;
	try {
	    curr = std::stoll(cell);
	} catch(const std::invalid_argument &e) {
	    std::cerr << "Error: Can't process: " << cell << '\n';
	    continue;
	}
	// Add each int in the program
	program.push_back(curr);
    }
    return program;
}

template<typename ReturnT = void>
class Computer {
    static_assert(std::is_same<ReturnT, void>::value
		  || std::is_same<ReturnT, std::optional<long long>>::value);
private:
    std::vector<long long> &program;
    unsigned long i = 0;
    unsigned long relativeBase = 0;
public:
    Computer(std::vector<long long> &prog) : program{prog}
    {}

    auto getValue(long long mode, long long param)
    {
	if(mode == 0) {
	    // Positional mode
	    auto value = program[param];
	    return value;
	} else if(mode == 1) {
	    // Immediate mode
	    return param;
	} else if(mode == 2) {
	    // Relative mode
	    auto value  = program[relativeBase + param];
	    return value;
	} else {
	    std::cerr << "Error: unknown mode: " << mode << " for instruction: "
		      << param << '\n';
	    exit(1);
	}
    }

    void setValue(long long mode, long long param, long long value)
    {
	if(mode == 0) {
	    // Positional mode
	    program[param] = value;
	} else if(mode == 2) {
	    // Relative mode
	    program[relativeBase + param] = value;
	} else {
	    std::cerr << "Error: unknown set mode: " << mode << " for instruction: "
		      << param << '\n';
	    exit(1);
	}
    }
    
    ReturnT operator()(long long input = 0)
    {
	while(i < program.size()) {
	    auto[opcode, param1Mode, param2Mode, param3Mode] = parse(program[i]);
	    if(opcode == 1) {
		// Add
		auto a = getValue(param1Mode, program[i+1]);
		auto b = getValue(param2Mode, program[i+2]);
		setValue(param3Mode, program[i+3], a + b);
		i += 4;
	    } else if(opcode == 2) {
		// Multiply
		auto a = getValue(param1Mode, program[i+1]);
		auto b = getValue(param2Mode, program[i+2]);
		setValue(param3Mode, program[i+3], a * b);
		i += 4;
	    } else if(opcode == 3) {
		// Replace with input
		long long value;
		if constexpr(std::is_same<ReturnT,void>::value) {
		    // Accept input from std-in; user manually enters input 
		    std::cout << "Enter value: ";
		    std::cin >> value;
		} else {
		    // Accept input as arg (ex: like with amps problem) 
		    value = input;
		}    
		setValue(param1Mode, program[i+1], value);
		i += 2;
	    } else if(opcode == 4) {
		// Print value
		auto value = getValue(param1Mode, program[i+1]);
		i += 2;
		if constexpr(std::is_same<ReturnT, void>::value) {
		    // Print-to-std-out mod
		    std::cout << value << '\n';
		} else {
		    // Return value instead of printing (for amp connections)
		    return {value};
		}
	    } else if(opcode == 5) {
		// Jump if true
		auto condition = getValue(param1Mode, program[i+1]);
		if(condition != 0) {
		    auto value = getValue(param2Mode, program[i+2]);
		    i = value;
		} else {
		    i += 3;
		}
	    } else if(opcode == 6) {
		// Jump if false
		auto condition = getValue(param1Mode, program[i+1]);
		if(condition == 0) {
		    auto value = getValue(param2Mode, program[i+2]);
		    i = value;
		} else {
		    i += 3;
		}
	    } else if(opcode == 7) {
		// Less than
		auto a = getValue(param1Mode, program[i+1]);
		auto b = getValue(param2Mode, program[i+2]);
		if(a < b) {
		    setValue(param3Mode, program[i+3], 1);
		} else {
		    setValue(param3Mode, program[i+3], 0);
		}
		i += 4;
	    } else if(opcode == 8) {
		// Equal to
		auto a = getValue(param1Mode, program[i+1]);
		auto b = getValue(param2Mode, program[i+2]);
		if(a == b) {
		    setValue(param3Mode, program[i+3], 1);
		} else {
		    setValue(param3Mode, program[i+3], 0);
		}
		i += 4;
	    } else if(opcode == 9) {
		// Adjust relative base
		auto amount = getValue(param1Mode, program[i+1]);
		relativeBase += amount;
		i += 2;
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
	if constexpr(!std::is_same<ReturnT, void>::value) {
	   return {};
	}
    }
};

int main()
{
    auto program{loadProgram("input.txt")};
    Computer c(program);
    c();
    
    return 0;
}
