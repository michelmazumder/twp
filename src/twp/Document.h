#pragma once

#include <string>
#include <list>
#include <memory>
#include <vector>

#include "win/Window.h"
#include "tdm/AttributedRope.h"
#include "util/Debuggable.h"

namespace twp {

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

	class Document {

	protected:
		bool dirty;
		std::string name;
		tdm::AttributedRope text;
		size_t currentEditingPosition;
		bool defragCompleted;
		ViewPort view;

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