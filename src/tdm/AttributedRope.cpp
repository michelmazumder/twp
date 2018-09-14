#include "tdm/AttributedRope.h"
#include <cassert>
#include <iostream>
#include <exception>
#include "util/MethodLogger.h"

using namespace std;
using namespace tdm;

Cursor::Cursor(const Rope* _rope, size_t _position) 
	: rope(_rope), positionAbsolute(_position)
{}

long Cursor::distance(const Cursor& b) const {
	if(rope != b.rope) throw InvalidComparisonBetweenDifferentRopes();
	return positionAbsolute - b.positionAbsolute;
}

bool Cursor::operator== (const Cursor& b) const {
	if(rope != b.rope) throw InvalidComparisonBetweenDifferentRopes();
	return positionAbsolute == b.positionAbsolute;	
}

Cursor& Cursor::operator++ () {
	if(positionAbsolute < rope->size())
		positionAbsolute++;
	return *this;
}

Cursor& Cursor::operator-- () {
	if(positionAbsolute > 0)
		positionAbsolute--;
	return *this;
}

Attribute::Attribute(const AttributedRope *r, int tid, int _ncursesValue) 
	: _start(r, 0), _end(r, 0), _tid(tid), ncursesValue(_ncursesValue)
{}

Attribute::Attribute(const Cursor& activationPos, int tid, int _ncursesValue) :
	_start(activationPos), _end(activationPos), _tid(tid), ncursesValue(_ncursesValue)
{}

Attribute::Attribute(const Cursor& start, const Cursor& end, int tid, int _ncursesValue) :
	_start(start), _end(end), _tid(tid), ncursesValue(_ncursesValue)
{}

void Attribute::setStartPosition(const Cursor& s) {
	_start = s;
}

void Attribute::setEndPosition(const Cursor& e) {
	_end = e;
}

std::list<Attribute *> AttributedRope::getAttributesForPosition(size_t position) {
	std::list<Attribute *> rt;
	for(auto att : attributeList) {
		if(
			(att.start().positionAbsolute <= position) &&
			(att.end().positionAbsolute >= position) ) {
			rt.push_back(&att);
		}
	}
	return rt;
}

void AttributedRope::attributeOn(Attribute& attribute, size_t currentPosition) {
	util::MethodLogger m(__PRETTY_FUNCTION__);
	assert(currentPosition <= size());
	attribute.setStartPosition(getCursorForPosition(currentPosition));
	attribute.setEndPosition(getCursorForPosition(currentPosition));
	attributeList.push_back(attribute);
}

// in append mode
void AttributedRope::attributeOn(Attribute& attribute) {
	util::MethodLogger m(__PRETTY_FUNCTION__);
	attributeOn(attribute, size() - 1);
}

void AttributedRope::attributeOff(Attribute& attribute, size_t currentPosition) {
	util::MethodLogger m(__PRETTY_FUNCTION__);
	assert(currentPosition <= size());
	attribute.setEndPosition(getCursorForPosition(currentPosition-1));
	attributeList.push_back(attribute);
}

// in append mode
void AttributedRope::attributeOff(Attribute& attribute) {
	attributeOff(attribute, size() - 1);
}

Cursor AttributedRope::getCursorForPosition(size_t position) {
	assert(position <= size());
	return Cursor(this, position);
}

void AttributedRope::append(CharType character) {
	Rope::append(character);
}

void AttributedRope::insertAt(CharType character, size_t position) {
	Rope::insertAt(character, position);
}

void AttributedRope::removeAt(size_t position) {
	Rope::removeAt(position);
}
