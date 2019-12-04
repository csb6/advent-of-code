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
	    std::cerr << "Error: unknown direction: " << direction << '\n';
	    exit(1);
	}
    }
    return wire;
}

int main()
{
    auto wire1{loadFile("input.txt")};
    std::sort(wire1.begin(), wire1.end(),
	      [](auto &a, auto &b) {
		  if(a.first == b.first) {
		      return a.second < b.second;
		  } else {
		      return a.first < b.first;
		  }
	      });
    auto wire2{loadFile("input2.txt")};
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
    //for(auto e : intersections) { std::cout << e.first << ' ' << e.second << '\n'; }
    auto closest = std::min_element(intersections.begin(), intersect_end,
				    [](auto &a, auto &b) {
					return std::abs(a.first) + std::abs(a.second)
					    < std::abs(b.first) + std::abs(b.second);
				    });
    std::cout << "Closest intersection: " << closest->first << ' ' << closest->second << '\n';
    std::cout << "Its Manhattan distance from (0,0): "
	      << std::abs(closest->first) + std::abs(closest->second) << '\n';
    return 0;
}
