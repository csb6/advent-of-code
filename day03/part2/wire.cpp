#include <iostream>
#include <fstream>
#include <vector>

auto loadFile(const std::string path)
{
    std::ifstream file(path);
    if(!file) { std::cerr << "Can't open: " << path << '\n'; exit(1);}
    std::vector<std::pair<int,int>> wire;
    int currX = 0;
    int currY = 0;
    while(file) {
	std::string cell;
	std::getline(file, cell, ',');	
	if(cell.size() == 0 || cell == "\n") continue;
        int distance = 0;
	try {
	    distance = std::stoi(cell.substr(1));
	} catch(const std::invalid_argument &e) {
	    std::cerr << "Error: Can't process: " << cell << '\n';
	    continue;
	}
	char direction = cell[0];
	if(direction == 'R') {
	    for(int i = 1; i <= distance; ++i) {
		wire.push_back({currX+i,currY});
	    }
	    currX += distance;
	} else if(direction == 'L') {
	    for(int i = 1; i <= distance; ++i) {
		wire.push_back({currX-i,currY});
	    }
	    currX -= distance;
	} else if(direction == 'U') {
	    for(int i = 1; i <= distance; ++i) {
		wire.push_back({currX,currY+i});
	    }
	    currY += distance;
	} else if(direction == 'D'){
	    for(int i = 1; i <= distance; ++i) {
		wire.push_back({currX,currY-i});
	    }
	    currY -= distance;
	} else {
	    std::cerr << "Error: Unknown direction" << direction << '\n';
	    exit(1);
	}
    }
    return wire;
}

auto getCircuitDistance(const std::vector<std::pair<int,int>> &wire1,
			const std::vector<std::pair<int,int>> &wire2,
			const std::pair<int,int> &point)
{
    auto side1_it = std::find_if(wire1.begin(),
				 wire1.end(), [&point](auto &a) {
						  return a.first == point.first
						      && a.second == point.second;
					      });
    auto side2_it = std::find_if(wire2.begin(),
				 wire2.end(), [&point](auto &a) {
						  return a.first == point.first
						      && a.second == point.second;
					      });
    auto steps1{std::distance(wire1.begin(), side1_it)};
    auto steps2{std::distance(wire2.begin(), side2_it)};
    return steps1 + steps2 + 2;
}

int main()
{
    auto origWire1{loadFile("input.txt")};
    auto wire1{origWire1};
    std::sort(wire1.begin(), wire1.end(),
	      [](auto &a, auto &b) {
		  if(a.first == b.first) {
		      return a.second < b.second;
		  } else {
		      return a.first < b.first;
		  }
	      });
    auto origWire2{loadFile("input2.txt")};
    auto wire2{origWire2};
    std::sort(wire2.begin(), wire2.end(),
	      [](auto &a, auto &b) {
		  if(a.first == b.first) {
		      return a.second < b.second;
		  } else {
		      return a.first < b.first;
		  }
	      });
    //for(auto e : wire1) { std::cout << e.first << ' ' << e.second << '\n'; }

    std::vector<std::pair<int,int>> intersections{std::max(wire1.size(), wire2.size())};
    auto intersect_end = std::set_intersection(wire1.begin(), wire1.end(),
			  wire2.begin(), wire2.end(),
			  intersections.begin());
    intersect_end = std::remove_if(intersections.begin(), intersect_end,
				   [](auto &a) {
				       return a.first == 0 && a.second == 0; 
				   });
    //for(auto e : wire1) { std::cout << e.first << ' ' << e.second << ','; }
    auto closest = std::min_element(intersections.begin(), intersect_end,
				    [&origWire1, &origWire2](auto &a, auto &b) {
				        auto stepsA{getCircuitDistance(origWire1,
								       origWire2, a)};
					auto stepsB{getCircuitDistance(origWire1,
								       origWire2, b)};
					return stepsA < stepsB;
				    });
    std::cout << "Closest intersection (by steps): " << closest->first << ' '
	      << closest->second << '\n';
    auto steps{getCircuitDistance(origWire1, origWire2, *closest)};
    std::cout << "Number of steps away from start: " << steps << '\n';
    return 0;
}
