#pragma once

#include <memory>
#include <list>
#include <string>

#include "tdm/Chunk.h"
#include "tdm/Rope.h"
#include "util/Debuggable.h"


namespace tdm {

	class InvalidComparisonBetweenDifferentRopes : public std::exception
	{};

	struct Cursor : public util::Debuggable {
		const Rope *rope;
		size_t positionAbsolute;

		Cursor(const Rope* rope, size_t position);

		long distance(const Cursor& b) const;
		bool operator== (const Cursor& b) const;
		Cursor& operator++ ();
		Cursor& operator-- ();

		virtual std::ostream& debugDump(std::ostream& debugStream) const;
	};

	class AttributedRope;

	class Attribute : public util::Debuggable {

	protected:
		Cursor _start;
		Cursor _end;
		int _tid;
		int ncursesValue;
		bool _isClosed;

	public:
		Attribute(const Attribute& b);
		Attribute(const AttributedRope *rope, int tid, int ncursesValue);
		Attribute(const Cursor& activationPos, int tid, int ncursesValue);
		Attribute(const Cursor& start, const Cursor& end, int tid, int ncursesValue);

		Attribute& operator = (const Attribute& b);

		void setStartPosition(const Cursor& start);
		void setEndPosition(const Cursor& end);
		void increaseEnd();
		void close();
		inline bool isClosed() const { return _isClosed; }

		inline const Cursor& start() const { return _start; }
		inline const Cursor& end() const { return _end; }
		inline Cursor& start() { return _start; }
		inline Cursor& end() { return _end; }
		inline int getTypeId() const { return _tid; }
		inline int getNcursesValue() const { return ncursesValue; }

		virtual std::ostream& debugDump(std::ostream& debugStream) const;
	};


	class AttributedRope : public Rope {
		
		std::list<std::shared_ptr<Attribute>> attributeList;

	public:
		void attributeOn(const Attribute& attribute, size_t currentPosition);
		void attributeOn(const Attribute& attribute); // in append mode
		void attributeOff(const Attribute& attribute, size_t currentPosition);
		void attributeOff(const Attribute& attribute); // in append mode

		Cursor getCursorForPosition(size_t position);
		const std::list<std::shared_ptr<Attribute>> getAttributesForPosition(size_t position) const;

		virtual void append(CharType character);
		virtual void insertAt(CharType character, size_t position);
		virtual void removeAt(size_t position);

		virtual std::ostream& debugDump(std::ostream& debugStream) const;
	};

}