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
    std::array<char,Rows*Cols> output;
    for(int row = 0; row < Rows; ++row) {
	for(int col = 0; col < Cols; ++col) {
	    auto color = std::find_if(layers.begin(), layers.end(),
				      [row, col](auto &each) {
					  return each[Cols * row + col] == '0'
					      || each[Cols * row + col] == '1';
				      });
	    
	    output[Cols * row + col] = (*color)[Cols * row + col];
	}
    }

    int i = 0;
    for(char each : output) {
	if(each == '0')
	    std::cout << ' ';
	else
	    std::cout << each;
	++i;
	if(i % Cols == 0) {
	    std::cout << '\n';
	}
    }

    return 0;
}
