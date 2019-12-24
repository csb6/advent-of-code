#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <array>
#include <cmath>

bool equal(double a, double b)
{
    return std::abs(a - b) <= 0.000001;
}

int countVisible(const std::vector<std::array<int,2>> &asteroids, int x, int y)
{
    int count = 0;
    std::vector<double> anglesSoFar;
    for(int i = 0; i < asteroids.size(); ++i) {
	if(asteroids[i][0] == x && asteroids[i][1] == y)
	    continue;
	double angle = std::atan2(asteroids[i][1] - y, asteroids[i][0] - x);
	auto match = std::find_if(anglesSoFar.begin(), anglesSoFar.end(),
				  [angle](double each) {
				      return equal(angle, each);
				  });
	if(match == anglesSoFar.end()) {
	    anglesSoFar.push_back(angle);
	    ++count;
	}
    }
    return count;
}

int main()
{
    std::vector<std::array<int,2>> asteroids;
    std::ifstream file("input.txt");
    int row = 0;
    while(file) {
	std::string line;
	std::getline(file, line);
	int col = 0;
	for(char each : line) {
	    if(each == '#') {
		asteroids.push_back({col, row});
	    }
	    ++col;
	}
	++row;
    }

    auto best_asteroid = std::max_element(asteroids.begin(), asteroids.end(),
					  [&asteroids](auto &a, auto &b) {
					      return countVisible(asteroids,
								  a[0], a[1])
						  < countVisible(asteroids,
								 b[0], b[1]);
					  });
    
    if(best_asteroid != asteroids.end()) {
	std::cout << "Asteroid that can see the most other asteroids: "
		  << (*best_asteroid)[0] << ", " << (*best_asteroid)[1] << '\n';
	std::cout << "You can see "
		  << countVisible(asteroids, (*best_asteroid)[0],
				  (*best_asteroid)[1]) << " asteroids from it\n";
    }

    return 0;
}
