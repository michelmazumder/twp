#include "twp/Elements.h"

using namespace twp;

// --------------- BEGIN POSITION ----------------
Position::Position() : pElement(nullptr), relativePos(-1) {}
Position::Position(Element *el, int pos) : pElement(el), relativePos(pos) {}
Position::Position(const Position& p) : pElement(p.pElement), relativePos(p.relativePos) {}
Position& Position::operator= (const Position& p) {
	pElement = p.pElement;
	relativePos = p.relativePos;
	return *this;
}

bool Position::isValid() const { 
	return (pElement != nullptr) 
		&& (relativePos < pElement->getMaxRelativePos()); 
}

// --------------- END POSITION ----------------


// --------------- BEGIN TEXTSLICE ----------------
Position TextSlice::begin() const { return Position((Element *)this, 0); }
size_t TextSlice::getMaxRelativePos() const { return content.length(); }
// --------------- END TEXTSLICE ----------------



// --------------- BEGIN PARAGRAPH ----------------
Position Paragraph::begin() const {
	return Position((Element *) this, 0);
}

size_t Paragraph::getMaxRelativePos() const {
	return textElements.size();
}

// --------------- END PARAGRAPH ----------------