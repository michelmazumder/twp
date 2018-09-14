#pragma once

#include <string>
#include <list>
#include <memory>
#include "win/Window.h"
#include "tdm/AttributedRope.h"

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
		
		void save();
		void debugDump();
		void deleteAtCurrentPos();
		void defrag();
	};

}