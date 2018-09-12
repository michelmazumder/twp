#pragma once

#include <vector>
#include <string>

namespace util
{
	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
	std::vector<std::string> split(const std::string &s, char delim);
	std::vector<std::string> split(const std::string &s);

	int toInt(const std::string& s);
}
