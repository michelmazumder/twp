#pragma once

#include <string>
#include <list>
#include <memory>
#include "win/Window.h"
#include "tdm/AttributedRope.h"
#include "twp/Attributes.h"

namespace twp {

	class Document {

		struct ActiveAttributes {
			std::shared_ptr<tdm::Attribute> italic;
		};

		ActiveAttributes activeAttributes;
	
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
		void toggleItalic();
		
		void save();
		void debugDump();
		void deleteAtCurrentPos();
		void defrag();
	};

}