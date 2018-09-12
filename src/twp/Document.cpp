#include "twp/Document.h"
#include "util/MethodLogger.h"

using namespace twp;


Document::Document() 
	: currentEditingPosition(0), defragCompleted(true) {

}

void Document::toggleItalic() {

	if(activeAttributes.italic.get() == nullptr) {

		tdm::Cursor c = text.getCursorForPosition(currentEditingPosition);

		activeAttributes.italic = std::shared_ptr<tdm::Attribute>(new twp::Italic(c, c));
		text.toggle(activeAttributes.italic, currentEditingPosition);
	}
	else {
		text.toggle(activeAttributes.italic, currentEditingPosition);
		activeAttributes.italic = std::shared_ptr<tdm::Attribute>(nullptr);
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
	util::MethodLogger m(__PRETTY_FUNCTION__);
	m.log() << "Char to insert = " << c;
	m.log() << "Current editing position = " << currentEditingPosition;
	size_t beforeInsert = text.size();

	if(currentEditingPosition == text.size()) {
		m.log() << "Appending text";
		text.append(c);
		currentEditingPosition++;
	}
	else {
		m.log() << "Inserting at position";
		text.insertAt(c, currentEditingPosition++);
	}

	size_t afterInsert = text.size();

	debugDump();

	if(afterInsert != (beforeInsert+1)) {
		m.log() << "ERRORE NELLA STRUTTURA DELLA ROPE!!!";
	}
	assert(afterInsert == (beforeInsert+1));
	defragCompleted = false;
}

void Document::deleteAtCurrentPos() {
	util::MethodLogger m(__PRETTY_FUNCTION__);

	size_t sz = text.size();
	
	m.log() << "sz = " << sz;
	m.log() << "currentEditingPosition = " << currentEditingPosition;

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
		util::MethodLogger m(__PRETTY_FUNCTION__);
		m.log() << " ** * * * * * * * *   DEFRAG * * * * * * * * * * *";
		size_t sizeBeforeDefrag = text.size();
		//debugDump();
		defragCompleted = text.defrag();
		size_t sizeAfterDefrag = text.size();
		//debugDump();
		if(sizeBeforeDefrag != sizeAfterDefrag) {
			m.log() << "Size before defrag = " << sizeBeforeDefrag;
			m.log() << "Size after defrag = " << sizeAfterDefrag;
		}
		assert(sizeBeforeDefrag == sizeAfterDefrag);
	}
}

void Document::render(win::Window& w) {
	util::MethodLogger m(__PRETTY_FUNCTION__);

	bool cursorDone = false;
	w.moveCursorTo(win::Point(0,0));

	for(size_t currentRenderPos = 0; currentRenderPos < text.size(); currentRenderPos++) {
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
