#include <iostream>
#include "../../util/image.hpp"

constexpr int Rows = 6;
constexpr int Cols = 25;

int main()
{
    Image<Rows, Cols> image("input.txt");

    std::cout << "# of 1's * # of 2's in layer with smallest number of 0's: "
	      << image.checkSum() << '\n';
    
    return 0;
}
