
#include "SortArray.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

std::vector<int> read_csv(std::string filepath) {
	std::ifstream f(filepath.c_str());
	std::vector<int> values; 

	if (f.is_open()) {
		for (std::string line; std::getline(f, line); ) {
			if (isdigit(line[0])) {
				values.push_back(std::stoi(line));
			}
		}
	} else {
		perror("Error opening file");
	}

	return values;
}
