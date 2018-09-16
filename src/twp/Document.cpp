#include "twp/Document.h"
#include "util/MethodLogger.h"
#include "twp/Attributes.h"

using namespace twp;


Document::Document() 
	: currentEditingPosition(0), defragCompleted(true) {
}

void Document::toggleUnderline() {
	// recupero tutti gli attributi attivi nella currentEditingPosition

	bool wasActive = false;
	auto activeAttributes = text.getAttributesForPosition(currentEditingPosition);
	for(auto att : activeAttributes) {
		if(att->getTypeId() == Underline::TYPE_ID) {
			assert(!wasActive);
			att->setEndPosition(text.getCursorForPosition(currentEditingPosition - 1));
			wasActive = true;
		}
	}

	if(!wasActive) {
		Underline attr(&text);
		text.attributeOn(attr, currentEditingPosition);
	}
}

void Document::debugDump() {
	util::MethodLogger m(__PRETTY_FUNCTION__);
	m.log() << "Current document editing position: " << currentEditingPosition;
	text.debugDump();
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

	// debugDump();

	assert(afterInsert == (beforeInsert+1));
	defragCompleted = false;
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

		auto activeAttributes = text.getAttributesForPosition(currentEditingPosition);
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
		w.activateBlinking();
		w.print("_");
		w.deactivateBlinking();
	}
}
