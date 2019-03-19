#pragma once

#include <cstddef> // size_t
#include "tdm/CharType.h"
#include <string>
#include <memory>
#include "win/Window.h"
#include <list>

namespace tdm {

	class TreeAttributedRope {
	
		class Element {

		public:
			size_t _startingPos;
			virtual size_t length() const = 0;
			virtual void render(win::Window& win) const = 0;
			bool ownsPosition(size_t position) const;
			bool isPerfectlyBefore(const Element& b) const;
		};

		class Container : Element {
		protected:
			// Insiemi (startpos..len) disgiunti e contigui da verificare in fase di inserimento
			std::list<std::shared_ptr<Element>> _subElements;
		public:
			std::shared_ptr<Element> getElementAt(size_t position) const;			
		};

		class Text : public Element {
		protected:
			std::string _text;
		public:
			bool appendChar(CharType character);
			bool insertAt(CharType character, size_t position);
			virtual size_t length() const;
			virtual void render(win::Window& win) const;
		};

		class Bold : public Container {

		};

		class Underline : public Container {

		};


	public:
		virtual void append(CharType character);
		virtual void toggleBold();
		virtual void toggleUnderline();

		virtual void insertAt(CharType character, size_t position);
		virtual void removeAt(size_t position);
	};
}