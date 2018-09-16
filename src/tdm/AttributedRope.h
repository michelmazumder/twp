#pragma once

#include <memory>
#include <list>
#include <string>

#include "tdm/Chunk.h"
#include "tdm/Rope.h"


namespace tdm {

	class InvalidComparisonBetweenDifferentRopes : public std::exception
	{};

	struct Cursor {
		const Rope *rope;
		size_t positionAbsolute;

		Cursor(const Rope* rope, size_t position);

		long distance(const Cursor& b) const;
		bool operator== (const Cursor& b) const;
		Cursor& operator++ ();
		Cursor& operator-- ();
	};

	class AttributedRope;

	class Attribute {

	protected:
		Cursor _start;
		Cursor _end;
		int _tid;
		int ncursesValue;

	public:
		Attribute(const AttributedRope *rope, int tid, int ncursesValue);
		Attribute(const Cursor& activationPos, int tid, int ncursesValue);
		Attribute(const Cursor& start, const Cursor& end, int tid, int ncursesValue);

		void setStartPosition(const Cursor& start);
		void setEndPosition(const Cursor& end);

		inline const Cursor& start() const { return _start; }
		inline const Cursor& end() const { return _end; }
		inline Cursor& start() { return _start; }
		inline Cursor& end() { return _end; }
		inline int getTypeId() const { return _tid; }
		inline int getNcursesValue() const { return ncursesValue; }
	};


	class AttributedRope : public Rope {
		
		std::list<Attribute> attributeList;

	public:
		void attributeOn(Attribute& attribute, size_t currentPosition);
		void attributeOn(Attribute& attribute); // in append mode
		void attributeOff(Attribute& attribute, size_t currentPosition);
		void attributeOff(Attribute& attribute); // in append mode

		Cursor getCursorForPosition(size_t position);
		std::list<Attribute *> getAttributesForPosition(size_t position);

		virtual void append(CharType character);
		virtual void insertAt(CharType character, size_t position);
		virtual void removeAt(size_t position);
	};

}