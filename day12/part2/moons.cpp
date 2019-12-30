#include <iostream>
#include <fstream>
#include <array>
#include <string>

class Moon {
private:
    long m_pos;
    long m_vel = 0;
    friend void applyGravity(Moon &a, Moon &b);
public:
    Moon(long pos = 0) : m_pos(pos)
    {}

    void applyVelocity()
    {
	m_pos += m_vel;
    }

    bool operator==(const Moon &other) const
    {
        return m_pos == other.m_pos
	    && m_vel == other.m_vel;
    }

    bool operator!=(const Moon &other) const
    {
	return !(*this == other);
    }
};

void applyGravity(Moon &a, Moon &b)
{
    if(a.m_pos < b.m_pos) {
	++a.m_vel;
	--b.m_vel;
    } else if(a.m_pos > b.m_pos) {
	--a.m_vel;
	++b.m_vel;
    }
}

void stepSimulation(std::array<Moon,4> &moons)
{
    for(int i = 0; i < 2; ++i) {
	for(int j = 2; j < 4; ++j) {
	    applyGravity(moons[i], moons[j]);
	}
    }
    applyGravity(moons[0], moons[1]);
    applyGravity(moons[2], moons[3]);

    for(auto &each : moons) {
	each.applyVelocity();
    }
}

bool all_equal(const std::array<Moon,4> &a, const std::array<Moon,4> &b)
{
    for(int i = 0; i < 4; ++i) {
	if(a[i] != b[i])
	    return false;
    }
    return true;
}

long get_cycle_length(std::array<Moon,4> &orig_moons)
{
    auto moons{orig_moons};
    long count = 0;
    do {
	stepSimulation(moons);
	++count;
    } while(!all_equal(orig_moons, moons));
    return count;
}

long gcd(long a, long b)
{
    while(b != 0) {
	auto temp = b;
	b = a % b;
	a = temp;
    }
    return a;
}

long lcm(long a, long b)
{
    return std::abs(a * b / gcd(a, b);
}

int main()
{
    std::ifstream file("input.txt");
    std::array<Moon,4> x_positions;
    std::array<Moon,4> y_positions;
    std::array<Moon,4> z_positions;
    int i = 0;
    while(file) {
	std::string line;
	std::getline(file, line);
	if(line.size() < 3)
	    continue;
	
	auto div_x = line.find(' ');
	int xPos = std::stoi(line.substr(3, div_x));
	x_positions[i] = Moon(xPos);
	
	auto div_y = line.find(' ', div_x+1);
	int yPos = std::stoi(line.substr(div_x+3, div_y));
	y_positions[i] = Moon(yPos);
	
	int zPos = std::stoi(line.substr(div_y+3));
	z_positions[i] = Moon(zPos);
	++i;
    }

    long x_count = get_cycle_length(x_positions);
    long y_count = get_cycle_length(y_positions);
    long z_count = get_cycle_length(z_positions);
    
    std::cout << "It takes " << x_count << " steps to reach duplicate x-state\n";
    std::cout << "It takes " << y_count << " steps to reach duplicate y-state\n";
    std::cout << "It takes " << z_count << " steps to reach duplicate z-state\n";    
    std::cout << "Therefore, to reach a fully duplicate state, it takes "
	      << lcm(lcm(x_count, y_count), z_count) << " steps\n";
    
    return 0;
}
