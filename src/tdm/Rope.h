#pragma once

#include "tdm/Chunk.h"
#include <string>

namespace tdm {

	class Rope {
	protected:
		struct ChunkPos {
			const Chunk *chunk;
			size_t startingPos;
		};

		Chunk *firstChunk;

		ChunkPos getChunkForPosition(size_t position) const;

		void deleteChunkWithChecks(Chunk *delMe);
		const Chunk *lastChunk() const;
		const bool checkLinks() const;

	public:
		Rope();
		virtual ~Rope();

		void append(const std::string& text);
		virtual void append(CharType character);
		virtual void insertAt(CharType character, size_t position);
		virtual void removeAt(size_t position);

		size_t size() const;
		const CharType& get(size_t position) const;

		void debugDump() const;
		// defrag completed
		bool defrag();
		std::string toString() const;
	};
	
};