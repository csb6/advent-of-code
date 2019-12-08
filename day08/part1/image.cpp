#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <algorithm>

constexpr int Rows = 6;
constexpr int Cols = 25;

int main()
{
    std::ifstream file("input.txt");
    std::vector<std::array<char,Cols*Rows>> layers;
    std::array<char,Cols*Rows> currLayer;
    int i = 0;
    while(file) {
	char digit;
	file >> digit;
	if(digit == '\n') continue;
	if(i < Cols * Rows) {
	    currLayer[i] = digit;
	    ++i;
	} else {
	    // Done with this layer
	    layers.push_back(currLayer);
	    i = 0;
	}
    }

    auto minLayer = std::min_element(layers.begin(), layers.end(),
				     [](auto &a, auto &b) {
					 auto aCount = std::count(a.begin(), a.end(), '0');
					 auto bCount = std::count(b.begin(), b.end(), '0');
					 return aCount < bCount;
				     });
    auto oneDigits = std::count(minLayer->begin(), minLayer->end(), '1');
    auto twoDigits = std::count(minLayer->begin(), minLayer->end(), '2');

    std::cout << "# of 1's * # of 2's in layer with smallest number of 0's: "
	      << oneDigits * twoDigits << '\n';
    
    return 0;
}
