#include <iostream>
#include <string>

int main()
{
    int count = 0;
    for(int i = 134564; i <= 585159; ++i) {
	const std::string num{std::to_string(i)};
	bool has_double = false;
	bool no_decrease = true;
	for(int i = 0; i < num.size()-1; ++i) {
	    if(num[i+1] < num[i]) {
		// Skip this num
		no_decrease = false;
		break;
	    } else if(!has_double && num[i+1] == num[i]) {
		has_double = true;
	    }
	}
	if(has_double && no_decrease) {
	    count += 1;
	}
    }

    std::cout << count << " numbers fit the criteria on the given range\n";
    
    return 0;
}
