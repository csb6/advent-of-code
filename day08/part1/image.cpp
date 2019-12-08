#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <algorithm>

constexpr int Rows = 6;
constexpr int Cols = 25;

std::vector<std::array<char,Rows*Cols>> loadLayers(const char *path)
{
    std::ifstream file(path);
    std::vector<std::array<char,Rows*Cols>> layers;
    std::array<char,Rows*Cols> currLayer;
    int i = 0;
    while(file) {
	char digit;
        file >> digit;
	if(!std::isdigit(digit)) continue;
	if(i < Rows*Cols) {
	    currLayer[i] = digit;
	    ++i;
	} else {
	    file.putback(digit);
	    i = 0;
	    layers.push_back(currLayer);
	    currLayer = {};
	}
    }
    return layers;
}

int main()
{
    auto layers{loadLayers("input.txt")};

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
