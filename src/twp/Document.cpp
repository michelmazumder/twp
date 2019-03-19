#include "twp/Document.h"
#include "util/MethodLogger.h"
#include "twp/Attributes.h"

using namespace twp;


ViewPort::ViewPort() : firstPositionOnRope(0)
{}

Document::Document() 
	: currentEditingPosition(0), defragCompleted(true) 
{}

bool Document::toggleUnderline() {
	// recupero tutti gli attributi attivi nella currentEditingPosition
	util::MethodLogger m(__PRETTY_FUNCTION__);
	m.log() << "Current editing position = " << currentEditingPosition;
	bool wasActive = false;
	auto activeAttributes = text.getAttributesForPosition(currentEditingPosition);
	for(auto att : activeAttributes) {
		if(att->getTypeId() == Underline::TYPE_ID) {
			assert(!wasActive);
			att->setEndPosition(text.getCursorForPosition(currentEditingPosition));
			wasActive = true;
		}
	}

	bool rt;
	Underline attr(&text);
	if(!wasActive) {
		m.log() << "Underline ON!";
		text.attributeOn(attr, currentEditingPosition);
		rt = true;
	}
	else {
		m.log() << "Underline OFF!!";
		text.attributeOff(attr, currentEditingPosition);
		rt = false;
	}

	debugDump(std::cerr);
	return rt;
}

bool Document::toggleBold() {
	// recupero tutti gli attributi attivi nella currentEditingPosition
	util::MethodLogger m(__PRETTY_FUNCTION__);
	m.log() << "Current editing position = " << currentEditingPosition;
	bool wasActive = false;
	auto activeAttributes = text.getAttributesForPosition(currentEditingPosition);
	for(auto att : activeAttributes) {
		if(att->getTypeId() == Bold::TYPE_ID) {
			assert(!wasActive);
			att->setEndPosition(text.getCursorForPosition(currentEditingPosition));
			wasActive = true;
		}
	}

	bool rt;
	Bold attr(&text);
	if(!wasActive) {
		m.log() << "Bold ON!";
		text.attributeOn(attr, currentEditingPosition);
		rt = true;
	}
	else {
		m.log() << "Bold OFF!!";
		text.attributeOff(attr, currentEditingPosition);
		rt = false;
	}

	debugDump(std::cerr);
	return rt;
}

std::ostream& Document::debugDump(std::ostream& debugStream) const {
	debugStream << "**** DOCUMENT DUMP ****" << std::endl;
	debugStream << "Current document editing position: " << currentEditingPosition << std::endl;
	text.debugDump(debugStream);
	return debugStream;
}

void Document::left() {
	if(currentEditingPosition > 0) --currentEditingPosition;
}

void Document::right() {
	if(currentEditingPosition < text.size()) ++currentEditingPosition;
}

void Document::home() {
	currentEditingPosition = 0;
}

void Document::end() {
	currentEditingPosition = text.size();
}

void Document::insertChar(char c) {
	size_t beforeInsert = text.size();

	if(currentEditingPosition == text.size()) {
		text.append(c);
		currentEditingPosition++;
	}
	else {
		text.insertAt(c, currentEditingPosition++);
	}

	size_t afterInsert = text.size();

	assert(afterInsert == (beforeInsert+1));
	defragCompleted = false;
	debugDump(std::cerr);
}

void Document::deleteAtCurrentPos() {
	size_t sz = text.size();
	if(currentEditingPosition < sz) {
		text.removeAt(currentEditingPosition);
	}
	if(currentEditingPosition > sz) {
		currentEditingPosition = sz;
	}
	defragCompleted = false;
}

void Document::save() {
	util::MethodLogger m(__PRETTY_FUNCTION__);
	// SAVE to doc
	dirty = false;
}

void Document::defrag() {
	if(defragCompleted) {
		size_t sizeBeforeDefrag = text.size();
		//debugDump();
		defragCompleted = text.defrag();
		size_t sizeAfterDefrag = text.size();
		//debugDump();
		/*
		if(sizeBeforeDefrag != sizeAfterDefrag) {
			util::MethodLogger m(__PRETTY_FUNCTION__);
			m.log() << "Size before defrag = " << sizeBeforeDefrag;
			m.log() << "Size after defrag = " << sizeAfterDefrag;
		}
		*/
		assert(sizeBeforeDefrag == sizeAfterDefrag);
	}
}

void Document::render(win::Window& w) {
	util::MethodLogger m(__PRETTY_FUNCTION__);

	bool cursorDone = false;
	w.moveCursorTo(win::Point(0,0));

	for(size_t currentRenderPos = 0; currentRenderPos < text.size(); currentRenderPos++) {

		auto activeAttributes = text.getAttributesForPosition(currentRenderPos);
		m.log() << "Attributi alla posizione " << currentRenderPos << " = " << activeAttributes.size();
		for(auto att : activeAttributes) {
			if(att->start().positionAbsolute == currentRenderPos) {
				// attribute on
				m.log() << ">>>>>> attribute on!";
				w.attributeOn(att->getNcursesValue());
			}
			if(att->end().positionAbsolute == currentRenderPos) {
				// attribute off
				m.log() << "<<<<<< attribute off!";
				w.attributeOff(att->getNcursesValue());
			}
		}
		
		if(currentRenderPos == currentEditingPosition) {
			w.activateBlinking();
			cursorDone = true;
		}

		std::string tmp;
		tmp += (char) text.get(currentRenderPos);
		w.print(tmp);
		
		if(currentRenderPos == currentEditingPosition) {
			w.deactivateBlinking();
		}
	}

	if(!cursorDone) {
		m.log() << " * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *";
		m.log() << " * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *";
		m.log() << " * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *";
		m.log() << " * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *";
		m.log() << " * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *";
		m.log() << " * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *";
		m.log() << " * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *";
		m.log() << " * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *";
		m.log() << " * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *";
		m.log() << " * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *";
		m.log() << " * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *";
		m.log() << " * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *";
		m.log() << " * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *";
		m.log() << " * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *";
		m.log() << " * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *";
		m.log() << " * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *";
		w.activateBlinking();
		w.print("_");
		w.deactivateBlinking();
	}

	w.attributeOff(A_BOLD);
	w.attributeOff(A_UNDERLINE);
}

/*
void ViewPort::render(win::Window& w) const {
	util::MethodLogger m(__PRETTY_FUNCTION__);
	w.moveCursorTo(win::Point(0,0));

	for(size_t currentRenderPos = 0; currentRenderPos < text.size(); currentRenderPos++) {

		auto activeAttributes = text.getAttributesForPosition(currentRenderPos);
		m.log() << "Attributi alla posizione " << currentRenderPos << " = " << activeAttributes.size();
		for(auto att : activeAttributes) {
			if(att->start().positionAbsolute == currentRenderPos) {
				// attribute on
				m.log() << ">>>>>> attribute on!";
				w.attributeOn(att->getNcursesValue());
			}
			if(att->end().positionAbsolute == currentRenderPos) {
				// attribute off
				m.log() << "<<<<<< attribute off!";
				w.attributeOff(att->getNcursesValue());
			}
		}
		
		if(currentRenderPos == currentEditingPosition) {
			w.activateBlinking();
			cursorDone = true;
		}

		std::string tmp;
		tmp += (char) text.get(currentRenderPos);
		w.print(tmp);
		
		if(currentRenderPos == currentEditingPosition) {
			w.deactivateBlinking();
		}
	}
	w.attributeOff(A_BOLD);
	w.attributeOff(A_UNDERLINE);
}
*/



/*
	class ViewPort {
		size_t firstPositionOnRope;
		std::vector<std::string> lines;
		std::vector<size_t> beginOfLineReferencingPosition;

		// riga x colonna x valore attributo
		std::vector<std::vector<std::vector<int>>> attron;
		std::vector<std::vector<std::vector<int>>> attroff;

	public:
		ViewPort();
		void render(win::Window& w) const;
	};

*/