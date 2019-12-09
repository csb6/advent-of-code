#include <iostream>
#include "../../util/image.hpp"

constexpr int Rows = 6;
constexpr int Cols = 25;

int main()
{
    Image<Rows, Cols> message("input.txt");
    const auto& output{message.process()};

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
