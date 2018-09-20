#pragma once

#include <string>
#include <list>
#include <memory>
#include "win/Window.h"
#include "tdm/AttributedRope.h"
#include "util/Debuggable.h"

namespace twp {

	class Document {

	protected:
		bool dirty;
		std::string name;
		tdm::AttributedRope text;
		size_t currentEditingPosition;
		bool defragCompleted;

	public:
		Document();
		void render(win::Window& w);
		void insertChar(char c);
		
		void left();
		void right();
		void home();
		void end();
		void toggleUnderline();
		void toggleBold();
		
		void save();

		void deleteAtCurrentPos();
		void defrag();

		virtual std::ostream& debugDump(std::ostream& debugStream) const;
	};

}