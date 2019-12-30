#include <iostream>
#include <fstream>
#include <array>
#include <string>

class Moon {
private:
    long x_pos;
    long y_pos;
    long z_pos;
    long x_vel = 0;
    long y_vel = 0;
    long z_vel = 0;
    friend void applyGravity(Moon &a, Moon &b);
public:
    Moon(long x = 0, long y = 0, long z = 0) : x_pos(x), y_pos(y), z_pos(z)
    {}

    void applyVelocity()
    {
	x_pos += x_vel;
	y_pos += y_vel;
	z_pos += z_vel;
    }

    long potentialEnergy() const
    {
	return std::abs(x_pos) + std::abs(y_pos) + std::abs(z_pos);
    }

    long kineticEnergy() const
    {
	return std::abs(x_vel) + std::abs(y_vel) + std::abs(z_vel);
    }

    long totalEnergy() const
    {
	return potentialEnergy() * kineticEnergy();
    }
};

void updateVel(long &pos_a, long &pos_b, long &vel_a, long &vel_b)
{
    if(pos_a < pos_b) {
	++vel_a;
	--vel_b;
    } else if(pos_a > pos_b) {
	--vel_a;
	++vel_b;
    }
}

void applyGravity(Moon &a, Moon &b)
{
    updateVel(a.x_pos, b.x_pos, a.x_vel, b.x_vel);
    updateVel(a.y_pos, b.y_pos, a.y_vel, b.y_vel);
    updateVel(a.z_pos, b.z_pos, a.z_vel, b.z_vel);
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

int main()
{
    std::ifstream file("input.txt");
    std::array<Moon,4> moons;
    int i = 0;
    while(file) {
	std::string line;
	std::getline(file, line);
	if(line.size() < 3)
	    continue;
	auto div_x = line.find(' ');
	int xPos = std::stoi(line.substr(3, div_x));
	auto div_y = line.find(' ', div_x+1);
	int yPos = std::stoi(line.substr(div_x+3, div_y));
	int zPos = std::stoi(line.substr(div_y+3));
	moons[i++] = Moon(xPos, yPos, zPos);
    }

    for(int i = 0; i < 1000; ++i) {
        stepSimulation(moons);
    }

    long total_energy = 0;
    for(auto &each : moons) {
	total_energy += each.totalEnergy();
    }

    std::cout << "Total energy after 1000 steps: " << total_energy << '\n';
    
    
    return 0;
}
