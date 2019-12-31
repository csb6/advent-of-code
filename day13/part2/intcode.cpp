/*
  Note: in order to build/run this day, install the termbox library, which is
  used for running the game on a terminal interface. You can find it at:
  https://github.com/nsf/termbox. Make sure that "termbox.h" header file from
  repo is in the same directory as this file, and make sure to build/install
  termbox before building this day's code.
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>
#include <type_traits>
#include <optional>
#include "termbox.h"

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

void loadScreen(const char *path)
{
    std::ifstream file(path);
    int i = 0;
    tb_cell *buf = tb_cell_buffer();
    while(file) {
	std::string cell;
	std::getline(file, cell, ',');
	if(cell.size() < 1)
	    continue;
	auto letter = std::stoul(cell);
	if(letter == 98) {
	    tb_change_cell(i % tb_width(), i / tb_width(),
			   static_cast<uint32_t>(letter), TB_DEFAULT, TB_RED);
	} else if(letter == 87) {
	    buf[i] = {static_cast<uint32_t>(letter), TB_DEFAULT, TB_DEFAULT};
	} else {
	    buf[i] = {static_cast<uint32_t>(letter), TB_YELLOW, TB_DEFAULT};
	}
	++i;
    }
    tb_present();
}

unsigned long loadLastPos(const char *path)
{
    std::ifstream file(path);
    std::string text;
    std::getline(file, text);
    return std::stoul(text);
}

template<typename ReturnT = void>
class Computer {
    static_assert(std::is_same<ReturnT, void>::value
		  || std::is_same<ReturnT, std::optional<long long>>::value);
private:
    std::vector<long long> &program;
    unsigned long i;
    unsigned long relativeBase = 0;
public:
    Computer(std::vector<long long> &prog, int start = 0)
	: program{prog}, i(start)
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
	        static tb_event event{};
		long long value;
		do {
		    tb_poll_event(&event);
		    if(event.ch == 's') {
			std::ofstream screenFile("currScreen.txt");
			auto *buf = tb_cell_buffer();
			for(int i=0; i < tb_width() * tb_height(); ++i) {
			    screenFile << buf[i].ch << ',';
			}
			std::ofstream currStateFile("currState.txt");
			for(auto each : program) {
			    currStateFile << std::to_string(each) << ',';
			}
			std::ofstream currPosFile("currPos.txt");
			currPosFile << std::to_string(i);
			event.type = TB_EVENT_RESIZE;
		    }
		} while(event.type != TB_EVENT_KEY);
		if(event.key == TB_KEY_ARROW_LEFT) {
		    // Go left
		    value = -1;
		} else if(event.key == TB_KEY_ARROW_UP) {
		    // Don't move
		    value = 0;
		} else if(event.key == TB_KEY_ARROW_RIGHT) {
		    value = 1;
		} else if(event.key == TB_KEY_CTRL_C) {
		    tb_shutdown();
		    exit(1);
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
		return {};
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
    std::cout << "\tBREAKOUT\n"
	      << "Use arrows to move joystick. Press 's' at any time during play "
	      << "to save the game state to disk; note that this will overwrite "
	      << "the last save.\n\n"
	      << "Do you want to start the game over, or reload from the last save?\n"
	      << "s - Start Over\nr - Reload\n";
    char choice;
    std::cin >> choice;
    
    std::vector<long long> program;
    unsigned long i = 0;
    if(choice == 's') {
        program = loadProgram("input.txt");
    } else if(choice == 'r') {
	program = loadProgram("currState.txt");
	i = loadLastPos("currPos.txt");
    } else {
	std::cout << "Invalid choice\n";
	return 1;
    }
    Computer<std::optional<long long>> c(program, i);
    long long score = 0;
    tb_init();
    if(choice == 'r')
	loadScreen("currScreen.txt");
    while(true) {
	auto res = c();
	if(!res)
	    break;
	auto x = *res;
	auto y = *c();
	auto id = *c();
	if(x == -1 && y == 0) {
	    score = id;
	    continue;
	}
	if(id == 0) {
	    tb_change_cell(x, y, ' ', TB_DEFAULT, TB_DEFAULT);
	} else if(id == 1) {
	    tb_change_cell(x, y, 'W', TB_DEFAULT, TB_DEFAULT);
	} else if(id == 2) {
	    tb_change_cell(x, y, 'b', TB_DEFAULT, TB_RED);
	} else if(id == 3) {
	    tb_change_cell(x, y, '-', TB_YELLOW, TB_DEFAULT);
	} else {
	    tb_change_cell(x, y, '0', TB_YELLOW, TB_DEFAULT);
	}
	tb_present();
    }
    tb_shutdown();

    std::cout << "Final score: " << score << '\n';
    
    return 0;
}
