#include "tdm/TreeAttributedRope.h"

#include "util/MethodLogger.h"
#include <typeinfo>
#include <cassert>

using namespace tdm;
using namespace std;


// -----------   Text  ------------

bool TreeAttributedRope::Text::appendChar(CharType character) {
	_text += character;
	return true;
}

bool TreeAttributedRope::Text::insertAt(CharType character, size_t relativePosition) {
	assert(relativePosition >= 0);
	assert(relativePosition < length());
	_text.insert(relativePosition, 1, string(static_cast<char>(character)));
	return true;
}

size_t TreeAttributedRope::Text::length() const {
	return _text.length();
}

void TreeAttributedRope::Text::render(win::Window& win) const {

}

// ----------   Container   -----------

size_t TreeAttributedRope::Container::length() const {
	size_t totalSize = 0;

	for(auto elem : _subElements) {
		totalSize += elem->length();
	}

	return totalSize;
}

void TreeAttributedRope::Container::render(win::Window& win) const {

}

std::shared_ptr<TreeAttributedRope::Text::Element> TreeAttributedRope::Container::getElementAt(size_t position) const {
	assert(position >= 0);
	assert(position < length());

	size_t acc = 0;
	for(auto elem : _subElements) {
		if(position >= acc && position < (acc + elem->length())) {
			return elem;
		}
		else {
			acc += elem->length();
		}
	}

	assert(false);
	return std::shared_ptr<TreeAttributedRope::Text::Element>{};
}

bool TreeAttributedRope::Container::appendNewSubElement(std::shared_ptr<Element> newSubElement) {
	if(isPerfectlyBefore(*newSubElement)) {
		_subElements.push_back(newSubElement);
		return true;
	}
	return false;
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
