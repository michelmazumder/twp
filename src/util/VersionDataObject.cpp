#include "util/VersionDataObject.h"
#include "util/StringExtensions.h"

using namespace util;

VersionDataObject::VersionDataObject(const std::string& v) {
	auto vals = split(v, '.');
	for(auto it = vals.cbegin(); it != vals.cend(); ++it) {
		version.push_back(toInt(*it));
	}
}

VersionDataObject::VersionDataObject(const VersionDataObject &b) : version(b.version) {
}

VersionDataObject& VersionDataObject::operator= (const VersionDataObject &b) {
	version = b.version;
	return *this;
}

bool VersionDataObject::operator== (const VersionDataObject &b) const {
	if(version.size() != b.version.size()) return false;
	for(int i = 0; i < version.size(); i++) {
		if(version[i] != b.version[i]) return false;
	}
	return true;
}

bool VersionDataObject::operator< (const VersionDataObject &b) const {
	int i = 1;
	while(true) {
		if(b.version.size() < i) return false;
		if(version.size() < i) return true;

		if(version[i-1] < b.version[i-1]) return true;
		else if(version[i-1] == b.version[i-1]) ++i;
		else return false;
	}
}

bool VersionDataObject::operator> (const VersionDataObject &b) const {
	return b < (*this);
}

bool VersionDataObject::operator<= (const VersionDataObject &b) const {
	if( (*this) == b) return true;
	if( (*this) < b) return true;
	return false;
}

bool VersionDataObject::operator>= (const VersionDataObject &b) const {
	if( (*this) == b) return true;
	if( (*this) > b) return true;
	return false;
}

