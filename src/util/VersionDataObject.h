#pragma once

#include <string>
#include <vector>

namespace util {
	
	class VersionDataObject {
	protected:
		std::vector<int> version;

	public:
		VersionDataObject(const std::string& v);
		VersionDataObject(const VersionDataObject &b);

		VersionDataObject& operator= (const VersionDataObject &b);

		bool operator== (const VersionDataObject &b) const;
		bool operator< (const VersionDataObject &b) const;
		bool operator> (const VersionDataObject &b) const;
		bool operator<= (const VersionDataObject &b) const;
		bool operator>= (const VersionDataObject &b) const;
	};


}