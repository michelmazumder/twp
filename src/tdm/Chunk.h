#pragma once

#include "tdm/CharType.h"
#include <cstddef>
#include <exception>
#include "util/Debuggable.h"

namespace tdm {

	class InvalidPositionException : public std::exception {};

	class Chunk : public util::Debuggable {
			static const int CHUNK_SIZE = 8;

			CharType _unicodeChunk[CHUNK_SIZE];
			size_t _begin; 
			size_t _end;
			Chunk * _previous;
			Chunk * _next;

			void resetBeginIfNeeded();

		public:

			Chunk(Chunk * previous);
			Chunk(Chunk * previous, Chunk *next);

			inline const Chunk *next() const { return _next; }
			inline const Chunk *previous() const { return _previous; }
			void assignNextChunk(Chunk *newNext);
			void assignPreviousChunk(Chunk *newPrev);

			// real data size
			size_t size() const;
			size_t avail() const;

			const CharType& get(size_t position) const;

			void append(CharType character);

			CharType deleteFirstChar();
			CharType deleteLastChar();
			bool canInsertAtPositionZero() const;
			
			// splitta il chunk nella posizione di editing
			void insertAt(CharType character, size_t position);

			void trunkAt(size_t position);

			void compact();
			virtual std::ostream& debugDump(std::ostream& debugStream) const;
	};
};