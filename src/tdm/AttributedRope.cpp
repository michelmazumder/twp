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
	if(positionAbsolute <= rope->size())
		positionAbsolute++;
	return *this;
}

Cursor& Cursor::operator-- () {
	if(positionAbsolute > 0)
		positionAbsolute--;
	return *this;
}

std::ostream& Cursor::debugDump(std::ostream& debugStream) const {
	debugStream << "Cursor: @" << positionAbsolute;
	return debugStream;
}


Attribute::Attribute(const Attribute& b) : 
	_start(b._start), _end(b._end), 
	_tid(b._tid), ncursesValue(b.ncursesValue),
	_isClosed(b._isClosed)
{}

Attribute& Attribute::operator = (const Attribute& b) {
	_start = b._start;
	_end = b._end;
	_tid = b._tid;
	ncursesValue = b.ncursesValue;
	_isClosed = b._isClosed;
	return *this;
}

Attribute::Attribute(const AttributedRope *r, int tid, int _ncursesValue) 
	: _start(r, 0), _end(r, 0), _tid(tid), 
	ncursesValue(_ncursesValue), _isClosed(false)
{}

Attribute::Attribute(const Cursor& activationPos, int tid, int _ncursesValue) :
	_start(activationPos), _end(activationPos), _tid(tid), 
	ncursesValue(_ncursesValue), _isClosed(false)
{}

Attribute::Attribute(const Cursor& start, const Cursor& end, int tid, int _ncursesValue) :
	_start(start), _end(end), _tid(tid), 
	ncursesValue(_ncursesValue), _isClosed(false)
{}

void Attribute::close() {
	_isClosed = true;
}

std::ostream& Attribute::debugDump(std::ostream& debugStream) const {
	util::MethodLogger m(__PRETTY_FUNCTION__);
	debugStream << "Address: " << this << std::endl;
	debugStream << "Start: ";
	_start.debugDump(debugStream) << std::endl;
	debugStream << "End: ";
	_end.debugDump(debugStream) << std::endl
		<< "Tid: " << _tid << std::endl
		<< "ncursesValue: " << ncursesValue << std::endl;


	return debugStream;
}

void Attribute::setStartPosition(const Cursor& s) {
	_start = s;
}

void Attribute::setEndPosition(const Cursor& e) {
	_end = e;
}

void Attribute::increaseEnd() {
	util::MethodLogger m(__PRETTY_FUNCTION__);
	m.log() << "Incremento end dell'attributo:";
	++_end;
	m.log() << "Now end = " << _end.positionAbsolute;
}

const std::list<std::shared_ptr<Attribute>> AttributedRope::getAttributesForPosition(size_t position) const {
	util::MethodLogger m(__PRETTY_FUNCTION__);

	std::list<std::shared_ptr<Attribute>> rt;
	for(auto att : attributeList) {
		if(
			(att->start().positionAbsolute <= position) &&
			(att->end().positionAbsolute >= position) ) {
				rt.push_back(att);
				m.log() << "Attributo caricato: ";
				att->debugDump(std::cerr);
		}
	}
	// m.log() << "Trovati " << rt.size() << " attributi";
	return rt;
}

void AttributedRope::attributeOn(const Attribute& attribute, size_t currentPosition) {
	util::MethodLogger m(__PRETTY_FUNCTION__);
	assert(currentPosition <= size());
	auto attp = std::shared_ptr<Attribute>(new Attribute(attribute));
	m.log() << "Setting start and end positions for attribute: ";
	attp->setStartPosition(getCursorForPosition(currentPosition));
	attp->setEndPosition(getCursorForPosition(currentPosition));
	attp->debugDump(std::cerr);
	m.log() << "Adding attribute to list:";
	attributeList.push_back(attp);
	m.log() << "List size now is " << attributeList.size();
}

// in append mode
void AttributedRope::attributeOn(const Attribute& attribute) {
	util::MethodLogger m(__PRETTY_FUNCTION__);
	attributeOn(attribute, size() - 1);
}

void AttributedRope::attributeOff(const Attribute& attribute, size_t currentPosition) {
	util::MethodLogger m(__PRETTY_FUNCTION__);
	assert(currentPosition <= size());

	auto activeAttrs = getAttributesForPosition(currentPosition);
	for(auto att : activeAttrs) {
		if(!att->isClosed()) {
			if(att->getTypeId() == attribute.getTypeId()) {
				m.log() << "Chiudo attributo: ";
				att->debugDump(std::cerr);

				auto currentCursor = getCursorForPosition(currentPosition);
				if(att->start().distance(currentCursor) == 0) {
					// att->setEndPosition(att->start());
					m.log() << "RIMUOVO ATTRIBUTO";
					attributeList.remove(att);
				}
				else {
					att->setEndPosition(getCursorForPosition(currentPosition));
					att->close();
				}
				return;
			}
		}
	}
}

// in append mode
void AttributedRope::attributeOff(const Attribute& attribute) {
	attributeOff(attribute, size() - 1);
}

Cursor AttributedRope::getCursorForPosition(size_t position) {
	if(position > (size()+1)) {
		util::MethodLogger m(__PRETTY_FUNCTION__);
		m.log() << "errore posizione = " << position << ", dimensione doc = " << size();
	}
	assert(position <= (size()+1));
	return Cursor(this, position);
}

void AttributedRope::append(CharType character) {
	util::MethodLogger m(__PRETTY_FUNCTION__);
	m.log() << "@@@@ APPEND @@@@";
	auto atts = getAttributesForPosition(size());
	m.log() << "Attributi per posizione " << size() << ": " << atts.size();
	for(auto att : atts) {
		if(!att->isClosed()) {
			att->debugDump(std::cerr);
			att->increaseEnd();
			att->debugDump(std::cerr);
		}
	}
	Rope::append(character);
}

void AttributedRope::insertAt(CharType character, size_t position) {
	Rope::insertAt(character, position);
}

void AttributedRope::removeAt(size_t position) {
	Rope::removeAt(position);
}

std::ostream& AttributedRope::debugDump(std::ostream& debugStream) const {
	debugStream << "*** ATTRIBUTED ROPE: ***" << std::endl;
	Rope::debugDump(debugStream);
	debugStream << " - - - - Attributes - - - - " << std::endl;
	debugStream << "size = " << attributeList.size() << std::endl;
	for(auto att : attributeList) {
		att->debugDump(debugStream);
	}
	debugStream << " - - - - - - - - - - - - - - " << std::endl;
	return debugStream;
}