#pragma once

#include <memory>
#include <list>
#include <string>

#include "tdm/Chunk.h"
#include "tdm/Rope.h"


namespace tdm {

	struct Cursor {
		const Rope *rope;
		const Chunk *chunk;
		size_t positionRelative;
		size_t positionAbsolute;

		Cursor& operator++ ();
		Cursor& operator-- ();

		long distance(const Cursor& b) const;
		bool operator== (const Cursor& b) const;
	};


	class Attribute {

	protected:
		Cursor _start;
		Cursor _end;

	public:
		Attribute(const Cursor& start, const Cursor& end);
		void endIncrease();
		void endDecrease();
		void startIncrease();
		void startDecrease();

		const Cursor& start() const;
		const Cursor& end() const;
	};


	class AttributedRope : public Rope {
		
		std::list<std::shared_ptr<Attribute>> activeAttributes;

	public:
		void toggle(std::shared_ptr<Attribute> attribute, size_t currentPosition);
		Cursor getCursorForPosition(size_t position);

		virtual void append(CharType character);
		virtual void insertAt(CharType character, size_t position);
		virtual void removeAt(size_t position);
	};

}