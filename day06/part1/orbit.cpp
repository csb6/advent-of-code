#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <vector>

int countOrbits(const std::map<std::string,std::string> &orbits)
{
    int count = 0;
    for(auto&[child, parent] : orbits) {
	++count;
	std::string curr{parent};
	while(orbits.find(curr) != orbits.end()) {
	    ++count;
	    curr = orbits.at(curr);
	}
    }
    return count;
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

    std::cout << "Number of direct and indirect orbitsL " << countOrbits(orbits) << '\n';
     
    //for(auto e : orbits) { std::cout << e << '\n' }
    
    return 0;
}
