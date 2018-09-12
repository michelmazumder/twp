#include "tdm/AttributedRope.h"
#include <cassert>
#include <iostream>
#include <exception>
#include "util/MethodLogger.h"

using namespace std;
using namespace tdm;

long Cursor::distance(const Cursor& b) const {
	return positionAbsolute - b.positionAbsolute;
}

bool Cursor::operator== (const Cursor& b) const {
	return (rope == b.rope) &&
		(chunk == b.chunk) &&
		(positionRelative == b.positionRelative) &&
		(positionAbsolute == b.positionAbsolute);
}

Cursor& Cursor::operator++ () {
	if(positionAbsolute < rope->size()) {
		positionAbsolute++;

		if(positionRelative < chunk->size()) {
			positionRelative++;
		}
		else {
			assert(chunk->next() != nullptr); // perchè positionAbsolute < rope->size()
			chunk = chunk->next();
			positionRelative = 0;
		}
	}

	return *this;
}

Cursor& Cursor::operator-- () {
	if(positionAbsolute > 0) {
		positionAbsolute--;

		if(positionRelative > 0) {
			positionRelative--;
		}
		else {
			assert(chunk->previous() != nullptr);
			chunk = chunk->previous();
			positionRelative = chunk->size() - 1;
		}
	}

	return *this;
}

Attribute::Attribute(const Cursor& start, const Cursor& end) :
	_start(start), _end(end)
{}

void Attribute::endIncrease() {
	++_end;
}

void Attribute::endDecrease() {
	--_end;
}

void Attribute::startIncrease() {
	++_start;
}

void Attribute::startDecrease() {
	--_start;
}

Cursor AttributedRope::getCursorForPosition(size_t position) {
	assert(position < size());
	Cursor c;
	c.positionAbsolute = position;
	ChunkPos cp = getChunkForPosition(position);
	c.chunk = cp.chunk;
	c.positionRelative = position - cp.startingPos;
	assert(c.positionRelative <= c.chunk->size());
	return c;
}

void AttributedRope::toggle(std::shared_ptr<Attribute> attribute, size_t currentPosition) {
	auto it = std::find(activeAttributes.begin(), activeAttributes.end(), attribute);
	if(it != activeAttributes.end()) {
		// elemento presente, switch off
		activeAttributes.remove(attribute);
	}
	else {
		// switch on
		// arriva già con le coordinate giuste
		assert(attribute->start() == attribute->end());
		assert(attribute->start().positionAbsolute == currentPosition);

		activeAttributes.push_back(attribute);
	}
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
