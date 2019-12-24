#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <array>
#include <cmath>
#include <cfloat>

constexpr double Pi = 3.141592654;
constexpr double Start = Pi / 2.0; // in radians
constexpr int StationX = 22;
constexpr int StationY = 28;

bool equal(double a, double b)
{
    double diff = std::fabs(a - b);
    a = std::fabs(a);
    b = std::fabs(b);
    double largest = (b > a) ? b : a;
 
    if (diff <= largest * DBL_EPSILON)
        return true;
    return false;
}

double simplifyRadians(double radians)
{
    if(radians > 0.0 || equal(radians, 0.0)) {
	return radians;
    } else {
	return 2.0 * Pi + radians;
    }
}

double distance(double x1, double y1, double x2, double y2)
{
    return std::sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

bool laserOrder(double a, double b)
{
    a = simplifyRadians(a);
    b = simplifyRadians(b);

    if(equal(a, Start)) {
	return true;
    } else if(equal(b, Start)) {
	return false;
    } else if(a > Start && b > Start) {
	return a > b;
    } else if(a > Start && b < Start) {
	return false;
    } else if(a < Start && b > Start) {
	return true;
    } else if(a < Start && b < Start) {
	return a > b;
    } else {
	assert(false);
	return false;
    }
}

auto getByAngle(const std::vector<std::array<int,2>> &asteroids, int x, int y)
{
    std::map<double,std::vector<std::array<int,2>>,
	     bool(*)(double,double)> anglesSoFar(laserOrder);
    for(const auto &asteroid : asteroids) {
	if(asteroid[0] == x && asteroid[1] == y)
	    continue;
	double angle = std::atan2(y - asteroid[1], asteroid[0] - x);
	auto match = std::find_if(anglesSoFar.begin(), anglesSoFar.end(),
				  [angle](auto &pair) {
				      return equal(angle, pair.first);
				  });
	if(match == anglesSoFar.end()) {
	    anglesSoFar[angle].push_back(asteroid);
	} else {
	    anglesSoFar[match->first].push_back(asteroid);
	}
    }
    return anglesSoFar;
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

    auto byAngle{getByAngle(asteroids, StationX, StationY)};
    for(auto&[angle, angleSet] : byAngle) {
	std::sort(angleSet.begin(), angleSet.end(),
		  [](auto a, auto b) {
		      return distance(StationX, StationY, a[0], a[1])
			  < distance(StationX, StationY, b[0], b[1]);
		  });
    }

    int count = 0;
    while(count < 200) {
	for(auto&[angle, asteroidSet] : byAngle) {
	    if(asteroidSet.empty())
		continue;
	    ++count;
	    if(count != 200) {
		asteroidSet.erase(asteroidSet.begin());
	    } else {
		std::cout << "200th destroyed: " << (*asteroidSet.begin())[0]
			  << ',' << (*asteroidSet.begin())[1] << '\n';
		std::cout << "X * 100 + y: "
			  << (*asteroidSet.begin())[0] * 100 + (*asteroidSet.begin())[1]
			  << '\n';
		break;
	    }
	}
    }

    return 0;
}
