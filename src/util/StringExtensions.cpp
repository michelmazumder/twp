#include "util/StringExtensions.h"

#include <sstream>

namespace util
{
	using namespace std;

    vector<string> &split(const string &s, char delim, vector<string> &elems)
    {
        stringstream ss(s);
        string item;
        while (getline(ss, item, delim)) {
            elems.push_back(item);
        }
        return elems;
    }


    vector<string> split(const string &s, char delim)
    {
        vector<string> elems;
        split(s, delim, elems);
        return elems;
    }

	vector<string> split(const string &str)
	{
		stringstream ss(str);
		string buf;
		vector<string> tokens;
		while(ss >> buf) tokens.push_back(buf);
		return tokens;
	}

    int toInt(const std::string& s)
    {
        istringstream buffer(s);
        int value;
        buffer >> value;
        return value;
    }
}
