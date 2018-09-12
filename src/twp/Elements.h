#pragma once

#include <string>
#include <list>
#include <memory>

namespace twp {
	
	class Position;

	class Element {
		public:
			virtual Position begin() const = 0;
			virtual size_t getMaxRelativePos() const = 0;
	};

	class Position {
		Element *pElement;
		size_t relativePos;

	public:
		Position();
		Position(Element *el, int pos);
		Position(const Position& p);
		
		Position& operator= (const Position& p);
		bool isValid() const;
	};

	class TextSlice : public Element {
		protected:
			std::string content;

		public:
			virtual Position begin() const;
			virtual size_t getMaxRelativePos() const;
	};

	class Paragraph : public Element {
		protected:
			std::string attributes;
			std::list<std::shared_ptr<TextSlice>> textElements;

		public:
			virtual Position begin() const;
			virtual size_t getMaxRelativePos() const;
	};

	class Chapter : public Element {
		protected: 
			std::string title;
			Paragraph subtitle;
			std::list<std::shared_ptr<Paragraph>> paragraphs;

	};
}