#include <vector>
#include <array>
#include <fstream>
#include <algorithm>

// Represents an image encoded in the Elves' special format
// Based on code starting from: Day08
template<int Rows, int Cols>
class Image {
private:
    std::vector<std::array<char,Rows*Cols>> m_layers;
    std::array<char,Rows*Cols> m_image;
public:
    explicit Image(const char *path)
    {
	std::ifstream file(path);
	std::array<char,Rows*Cols> currLayer;
	int i = 0;
	while(file) {
	    char digit;
	    file >> digit;
	    if(!std::isdigit(digit)) continue;
	    if(i < Rows*Cols) {
		currLayer[i] = digit;
		++i;
	    } else {
		file.putback(digit);
		i = 0;
		m_layers.push_back(currLayer);
		currLayer = {};
	    }
	}
    }

    auto checkSum() const {
	auto minLayer = std::min_element(m_layers.begin(), m_layers.end(),
				     [](auto &a, auto &b) {
					 auto aCount = std::count(a.begin(), a.end(), '0');
					 auto bCount = std::count(b.begin(), b.end(), '0');
					 return aCount < bCount;
				     });
	auto oneDigits = std::count(minLayer->begin(), minLayer->end(), '1');
	auto twoDigits = std::count(minLayer->begin(), minLayer->end(), '2');
	return oneDigits * twoDigits;
    }

    const auto& process()
    {
	for(int row = 0; row < Rows; ++row) {
	    for(int col = 0; col < Cols; ++col) {
		auto color = std::find_if(m_layers.begin(), m_layers.end(),
					  [row, col](auto &each) {
					      return each[Cols * row + col] == '0'
						  || each[Cols * row + col] == '1';
					  });
		
	        m_image[Cols * row + col] = (*color)[Cols * row + col];
	    }
	}
	return m_image;
    }

    const auto& image() const { return m_image; }
};
