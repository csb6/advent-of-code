#include <iostream>
#include <string>
#include <algorithm>

bool fits(const std::string &num)
{
    bool has_double = false;
    bool no_decrease = true;
    for(unsigned int i = 0; i < num.size()-1; ++i) {
	if(num[i+1] < num[i]) {
	    no_decrease = false;
	    break;
	} else if(!has_double && num[i+1] == num[i]) {
	    if(i > 0 && i < num.size()-2
	       && (num[i-1] == num[i] || num[i+2] == num[i])) {
		has_double = false;
	    } else if(i == 0 && num[i+2] == num[i]){
		has_double = false;
	    } else if(i == num.size()-2 && num[i-1] == num[i]) {
		has_double = false;
	    } else {
		has_double = true;
	    }
	}
    }
    return has_double && no_decrease;
}

int main()
{
    int count = 0;
    for(int i = 134564; i <= 585159; ++i) {
	const std::string num{std::to_string(i)};
	if(fits(num)) {
	    ++count;
	}
    }
    std::cout << count << " numbers fit the criteria on the given range\n";
    
    return 0;
}
