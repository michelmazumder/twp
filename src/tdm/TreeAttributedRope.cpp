#include "tdm/TreeAttributedRope.h"

#include "util/MethodLogger.h"
#include <typeinfo>
#include <cassert>

using namespace tdm;
using namespace std;

// -----------   Element   ---------------

bool TreeAttributedRope::Element::ownsPosition(size_t position) const {
	return (_startingPos <= position) && ((_startingPos + length()) > position);
}

bool TreeAttributedRope::Element::isPerfectlyBefore(const Element& b) const {
	return b._startingPos == (_startingPos + length() + 1);	
}

// -----------   Text  ------------

bool TreeAttributedRope::Text::appendChar(CharType character) {
	_text += character;
	return true;
}

bool TreeAttributedRope::Text::insertAt(CharType character, size_t position) {
	if(position < _startingPos) {
		return false;
	}
	if(position > (_startingPos + length())) {
		return false;
	}

	size_t relativePos = position - _startingPos;
	assert(relativePos >= 0);
	assert(relativePos < length());
	char c = (char) character;
	_text.insert(relativePos, 1, c);
	return true;
}

size_t TreeAttributedRope::Text::length() const {
	return _text.length();
}

void TreeAttributedRope::Text::render(win::Window& win) const {

}

// ----------   Container   -----------

size_t TreeAttributedRope::Container::length() const {

}

void TreeAttributedRope::Container::render(win::Window& win) const {
	
}

std::shared_ptr<TreeAttributedRope::Text::Element> TreeAttributedRope::Container::getElementAt(size_t position) const {
	for(auto mapEntry : _subElements) {
		if(mapEntry.second->ownsPosition(position)) {
			return mapEntry.second;
		}
	}
	return std::shared_ptr<TreeAttributedRope::Text::Element>{};
}




// ----------  TreeAttributedRope  -----------

void TreeAttributedRope::append(CharType character) {

}

void TreeAttributedRope::insertAt(CharType character, size_t position) {

}

void TreeAttributedRope::removeAt(size_t position) {
	
}

void TreeAttributedRope::toggleBold() {

}

void TreeAttributedRope::toggleUnderline() {

}
