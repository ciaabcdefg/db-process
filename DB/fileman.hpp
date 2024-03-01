#pragma once

#include <vector>
#include <string>
#include <unordered_set>

namespace fileman {
	bool has_duplicates(std::vector<std::string>& vec) {
		std::unordered_set<std::string> set;

		for (int i = 0; i < vec.size(); i++) {
			auto& str = vec[i];

			if (set.contains(str)) {
				std::cout << "Duplicate found: ";
				std::cout << "Line " << i + 1 << '\n';

				return true;
			}

			set.insert(str);
		}

		return false;
	}
}

