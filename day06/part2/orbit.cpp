#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <vector>

int shortestDistance(const std::map<std::string,std::string> &orbits,
		     std::string start, std::string end)
{
    std::vector<std::string> endSoFar;
    while(orbits.find(end) != orbits.end()) {
        end = orbits.at(end);
	endSoFar.push_back(end);
    }

    int startSoFarCount = 0;
    while(orbits.find(start) != orbits.end()) {
	start = orbits.at(start);
	for(int i = 0; i < endSoFar.size(); ++i) {
	    if(start == endSoFar[i]) {
		return startSoFarCount + i;
	    }
	}
	++startSoFarCount;
    }
    return -1;
}

int main()
{
    std::ifstream file("input.txt");
    std::map<std::string,std::string> orbits;
    while(file) {
	std::string line;
	std::getline(file, line);
	if(line.size() < 7) continue;
	int i = 0;
	while(line[i] != ')') {
	    ++i;
	}
	auto key = line.substr(i+1);
	auto value = line.substr(0, i);
	assert(orbits.find(key) == orbits.end());
	orbits[key] = value;
    }

    std::cout << "Shortest distance to Santa: "
	      << shortestDistance(orbits, "YOU", "SAN") << '\n';    
    return 0;
}
