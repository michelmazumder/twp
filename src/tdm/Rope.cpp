#include "tdm/Rope.h"
#include "util/MethodLogger.h"

using namespace tdm;
using namespace std;

Rope::Rope() 
{
	firstChunk = new Chunk(nullptr);
}

Rope::~Rope() {
	Chunk *deleteMe = firstChunk;
	while(deleteMe != nullptr) {
		Chunk *nxt = const_cast<Chunk *>(deleteMe->next());
		delete deleteMe;
		deleteMe = nxt;
	}
}

void Rope::append(CharType character) {
	const_cast<Chunk *>(lastChunk())->append(character);
	assert(checkLinks());
}

void Rope::append(const std::string& text) {
	for(auto c : text) {
		append(c);
	}
}

size_t Rope::size() const {
	size_t rt = 0;
	const Chunk *c = firstChunk;
	while(c != nullptr) {
		rt += c->size();
		c = c->next();
	}
	return rt;
}

const CharType& Rope::get(size_t position) const {
	assert(position < size());
	if(position >= size()) throw InvalidPositionException(); 

	ChunkPos c = getChunkForPosition(position);
	return c.chunk->get(position - c.startingPos);
}

Rope::ChunkPos Rope::getChunkForPosition(size_t position) const {
	assert(position < size());
	if(position >= size()) throw InvalidPositionException(); 

	ChunkPos rt;
	rt.chunk = firstChunk;
	rt.startingPos = 0;
	while(rt.chunk != nullptr) {
		if(position >= rt.chunk->size()) {
			position -= rt.chunk->size();
			rt.startingPos += rt.chunk->size();
			assert(rt.chunk->next() != nullptr);
			rt.chunk = rt.chunk->next();
		}
		else {
			return rt;
		}
	}
	return rt;
}

void Rope::insertAt(CharType character, size_t position) {
	if(position == size()) {
		append(character);
	}
	else if(position == 0) { // && size() > 0
		assert(size() > 0);
		Chunk *newFirst = new Chunk(nullptr, firstChunk);
		newFirst->append(character);
		assert(firstChunk->previous() == nullptr);
		firstChunk->assignPreviousChunk(newFirst);
		firstChunk = newFirst;
	}
	else {
		assert(position <= size());
		if(position > size()) throw InvalidPositionException();
		ChunkPos c = getChunkForPosition(position);
		assert(c.chunk != nullptr);
		size_t insPosInChunk = position - c.startingPos;
		const_cast<Chunk *>(c.chunk)->insertAt(character, insPosInChunk);
	}
	assert(checkLinks());
}

void Rope::removeAt(size_t position) {
	if(size() < 1) return;

	assert(position < size());
	if(position >= size()) throw InvalidPositionException();

	ChunkPos affectedChunk = getChunkForPosition(position);
	
	assert(affectedChunk.chunk != nullptr);
	assert(affectedChunk.chunk->size() > 0);
	assert(position >= affectedChunk.startingPos);
	
	auto relativePos = position - affectedChunk.startingPos;

	assert(relativePos < affectedChunk.chunk->size());

	Chunk *modChunk = const_cast<Chunk *>(affectedChunk.chunk);
	if(relativePos == 0) {
		modChunk->deleteFirstChar();

		if(modChunk->size() == 0 && modChunk != firstChunk) {
			// cancello questo chunk
			deleteChunkWithChecks(modChunk);
		}
	}
	else if(relativePos == (affectedChunk.chunk->size() - 1)) {
		modChunk->deleteLastChar();
	}
	else {
		// devo separare in due chunk
		Chunk *secondPart = new Chunk(modChunk, const_cast<Chunk *>(modChunk->next()));
		for(auto i = relativePos+1 ; i < affectedChunk.chunk->size(); i++) {
			auto cxx = affectedChunk.chunk->get(i);
			secondPart->append(cxx);
		}
		modChunk->assignNextChunk(secondPart);
		modChunk->trunkAt(relativePos);
	}
	assert(checkLinks());
}

void Rope::deleteChunkWithChecks(Chunk *delMe) {
	if(delMe == nullptr) return;

	assert(delMe->previous() != nullptr);
	assert(delMe->size() == 0);

	const_cast<Chunk *>(delMe->previous())
		->assignNextChunk( const_cast<Chunk *>(delMe->next()) );
	
	if(delMe->next() != nullptr)
		const_cast<Chunk *>(delMe->next())
			->assignPreviousChunk(const_cast<Chunk *>(delMe->previous()));
	delete delMe;
}

void Rope::debugDump() const {
	cerr << "****** DEBUG DUMP ******" << endl;
	cerr << "Dimensione totale = " << size() << endl;

	const Chunk *c = firstChunk;
	int counter = 1;
	while(c != nullptr) {
		cerr << "\tChunk n. " << counter << endl;
		c->debugDump();
		c = c->next();
		counter++;
	}
}

bool Rope::defrag() {
	size_t sizeBefore = size();
	const Chunk *current = firstChunk;

	while(current != nullptr) {
		if((current != firstChunk) && (current->size() == 0)) {
			deleteChunkWithChecks(const_cast<Chunk *>(current));
			assert(sizeBefore == size());
			assert(checkLinks());
			return false;
		}

		const_cast<Chunk *>(current)->compact();
		assert(sizeBefore == size());

		while( (current->avail() > 0) && (current->next() != nullptr) && (current->next()->size() > 0)) {
			// vado a prendere dati dal successivo e li copio qui
			const_cast<Chunk *>(current)->append(current->next()->get(0));
			const_cast<Chunk *>(current->next())->deleteFirstChar();
		}
		assert(sizeBefore == size());

		if(current->avail() > 0) {
			// ha dello spazio
			if(current->next() != nullptr) {
				if(current->next()->size() < current->avail()) {
					// riesco a compattarne 2 -> 1
					while(current->next()->size() > 0) {
						const_cast<Chunk *>(current)->append(
							const_cast<Chunk *>(current->next())->deleteFirstChar());
					}
					deleteChunkWithChecks(const_cast<Chunk *>(current->next()));
					assert(checkLinks());
					return false;
				}
			}
		}
		current = current->next();
	}
	assert(sizeBefore == size());
	assert(checkLinks());
	return true; // defrag completed
}

std::string Rope::toString() const {
	std::string rt;
	for(size_t i = 0; i < size(); i++) {
		rt += get(i);
	}
	return rt;
}

const Chunk * Rope::lastChunk() const {
	const Chunk *current = firstChunk;
	while(current->next() != nullptr) current = current->next();
	return current;	
}

const bool Rope::checkLinks() const {
	const Chunk * c = firstChunk;
	while (c != nullptr) {
		if(c->next() != nullptr) {
			if(c->next()->previous() != c) {
				util::MethodLogger m(__PRETTY_FUNCTION__);
				m.log() << "Controllo link fallito: ";
				m.log() << "THIS = ";
				c->debugDump();
				m.log() << "NEXT = ";
				c->next()->debugDump();
				assert(c->next()->previous() == c);
				return false;
			}
		}
		c = c->next();
	}
	return true;
}


