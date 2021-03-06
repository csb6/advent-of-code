#include <cstdio>
#include <iostream>
#include <fstream>
#include <cmath>

long double calculateFuelUsage(long double fuel)
{
    long double temp = std::floor(fuel / 3) - 2;
    if(temp <= 0) {
	return 0;
    } else {
	return temp + calculateFuelUsage(temp);
    }
}

int main()
{
    std::ifstream file{"fuel.txt"};
    long double total_fuel = 0;
    while(file) {
	std::string line;
	std::getline(file, line);
	if(line.size() == 0) {
	    continue;
	}
	long double module_fuel;
	try {
	    module_fuel = std::floor(std::stod(line) / 3) - 2;
	} catch(const std::invalid_argument &e) {
	    std::cerr << "Error: couldn't process: " << line << '\n';
	    continue;
	}
	total_fuel += module_fuel + calculateFuelUsage(module_fuel);
    }
    std::printf("%.7Lf\n", total_fuel);
    
    return 0;
}
