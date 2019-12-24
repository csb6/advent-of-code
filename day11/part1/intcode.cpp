#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>
#include <type_traits>
#include <optional>
#include <set>

std::array<long long, 4> parse(int instruction)
{
    auto opcode = instruction % 100;
    auto param1Mode = (instruction / 100) % 10;
    auto param2Mode = (instruction / 1000) % 10;
    auto param3Mode = (instruction / 10000) % 10;
    return {opcode, param1Mode, param2Mode, param3Mode};
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
		std::cout << "Done running\n";
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


class Direction {
private:
    enum Dir { Up = 0, Right = 1, Down = 2, Left = 3 };
    int m_current = Up;
public:
    void turn_right()
    {
	++m_current;
	if(m_current > Left) {
	    m_current = Up;
	}
    }

    void turn_left()
    {
	--m_current;
	if(m_current < Up) {
	    m_current = Left;
	}
    }

    int direction() const
    {
	if(m_current == Right) {
	    return 0;
	} else if(m_current == Up) {
	    return 90;
	} else if(m_current == Left) {
	    return 180;
	} else {
	    return 270;
	}
    }
};

int main()
{
    auto program{loadProgram("input.txt")};
    Computer<std::optional<long long>> c(program);
    Direction currDir;
    std::set<std::array<int,2>> black_tiles;
    std::set<std::array<int,2>> white_tiles;
    std::array<int,2> curr_tile{0, 0};
    while(true) {
	long long currColor = 0;
	if(white_tiles.find(curr_tile) != white_tiles.end())
	    currColor = 1;
	auto color = c(currColor);
	if(color.has_value() && *color == 0) {
	    black_tiles.insert(curr_tile);
	    if(white_tiles.find(curr_tile) != white_tiles.end())
		white_tiles.erase(curr_tile);
	} else if(color.has_value() && *color == 1) {
	    white_tiles.insert(curr_tile);
	    if(black_tiles.find(curr_tile) != black_tiles.end())
	        black_tiles.erase(curr_tile);
	} else {
	    break;
	}
	auto new_direction = c();
	if(new_direction.has_value() && *new_direction == 0) {
	    currDir.turn_left();
	} else if(new_direction.has_value() && *new_direction == 1) {
	    currDir.turn_right();
	} else {
	    break;
	}

	if(currDir.direction() == 0) {
	    curr_tile[0] += 1;
	} else if(currDir.direction() == 90) {
	    curr_tile[1] -= 1;
	} else if(currDir.direction() == 180) {
	    curr_tile[0] -= 1;
	} else {
	    curr_tile[1] += 1;
	}
    }

    std::cout << "Number of panels painted at least once: "
	      << black_tiles.size() + white_tiles.size() << '\n';
    
    return 0;
}
