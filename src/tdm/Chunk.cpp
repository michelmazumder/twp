#include "tdm/Chunk.h"
#include "util/MethodLogger.h"
#include <iostream>

using namespace tdm;
using namespace std;

Chunk::Chunk(Chunk * __previous)
	: Chunk(__previous, nullptr)
{
}

Chunk::Chunk(Chunk * prev, Chunk *nxt) : _begin(0), _end(0), _previous(prev), _next(nxt)
{}

void Chunk::assignNextChunk(Chunk *newNext) {
	_next = newNext;
}

void Chunk::assignPreviousChunk(Chunk *newPrev) {
	_previous = newPrev;
}

size_t Chunk::size() const {
	if(_end < _begin) {
		util::MethodLogger m(__PRETTY_FUNCTION__);
		m.log() << "chunk = " << this;
		m.log() << "begin = " << _begin;
		m.log() << "end = " << _end;
	}

	assert(_end >= _begin);
	return _end - _begin;
}

size_t Chunk::avail() const {
	return CHUNK_SIZE - _end;
}

void Chunk::resetBeginIfNeeded() {
	if(size() == 0) {
		_begin = 0;
		_end = 0;
	}	
}

void Chunk::compact() {
	util::MethodLogger m(__PRETTY_FUNCTION__);
	size_t beforeSize = size();
	if(size() == 0) {
		_begin = 0;
		_end = 0;
	}
	else if(_begin > 0) {
		m.log() << "begin = " << _begin;
		size_t gapSize = _begin;
		for(size_t i = 0; i < size(); i++) {
			_unicodeChunk[i] = _unicodeChunk[i+_begin];
		}
		assert(_end > _begin);
		_end -= _begin;
		_begin = 0;
	}
	m.log() << "Now compacted chunk:";
	debugDump();
	assert(_begin <= _end);
	assert(_end <= CHUNK_SIZE);
	assert(beforeSize == size());
}

const CharType& Chunk::get(size_t position) const {
	assert(_begin < _end);
	if(_begin >= _end) throw InvalidPositionException();
	size_t idx = position + _begin;
	
	assert(idx < _end);
	if(idx >= _end) throw InvalidPositionException();

	return _unicodeChunk[idx];
}

void Chunk::append(CharType character) {

	if(_end < _begin) {
		debugDump();
	}

	assert(_end >= _begin);

	if(_end < CHUNK_SIZE) {
		_unicodeChunk[_end] = character;
		_end++;
		assert(_end <= CHUNK_SIZE);
	}
	else {
		if(_next != nullptr) {
			_next->append(character);
		}
		else {
			_next = new Chunk(this);
			_next->append(character);
		}
	}
}

CharType Chunk::deleteFirstChar() {
	assert(size() > 0);
	CharType rt = _unicodeChunk[_begin];
	_begin++;
	assert(_begin <= _end);
	return rt;
}

CharType Chunk::deleteLastChar() {
	assert(size() > 0);
	_end--;
	CharType rt = _unicodeChunk[_end];
	assert(_begin <= _end);
	return rt;
}

bool Chunk::canInsertAtPositionZero() const {
	return _begin > 0;
}

void Chunk::trunkAt(size_t position) {
	if(position >= size()) {
		util::MethodLogger m(__PRETTY_FUNCTION__);
		m.log() << "Posizione richiesta = " << position;
		m.log() << "Dimensione chunk = " << size();
	}
	assert(position < size());
	_end = position;
	assert(_end >= _begin);
	assert(_end <= CHUNK_SIZE);
}

void Chunk::insertAt(CharType character, size_t position) {
	util::MethodLogger m(__PRETTY_FUNCTION__);
	assert(_begin < _end);
	if(_begin >= _end) throw InvalidPositionException();
	
	m.log() << "Posizione = " << position;
	m.log() << "size = " << size();
	m.log() << "sono il chunk " << this;

	assert(position < size());
	if(position >= size()) throw InvalidPositionException();

	if(position == 0 && canInsertAtPositionZero()) {
		m.log() << "inserisco all'inizio";
		assert(_begin > 0);
		_begin--;
		_unicodeChunk[_begin] = character;
	}
	else if(position == 0 && _previous != nullptr) {// && !canInsertAtPositionZero()
		// non sono il primo chunk

		// [prev] -> [io___]
        //           X

        // [prev] -> [X] -> [io___]

		Chunk *newOne = new Chunk(_previous, this);
		_previous->_next = newOne;
		newOne->append(character);
		this->_previous = newOne;
	}
	else if(avail() > 0 && position == size()) {
		m.log() << "Faccio una append";
		// append here
		append(character);
	}
	else {
		// deve splittare in due il chunk originale, creando un TERZO chunk di insert!
		m.log() << "Faccio la split, posizione di inserimento = " << position;

		// this [abcde___] -> [X]
		//         1
		// this [ab1_____] -> [cde_____] -> [X]

		Chunk *newEditingChunk = new Chunk(this, this->_next);
		if(_next != nullptr)
			_next->_previous = newEditingChunk;

		for(auto i = position; i < size(); i++) {
			m.log() << "i -> " << i;
			newEditingChunk->append(get(i));
		}

		_end = position+_begin;
		_next = newEditingChunk;

		append(character);
	}
}

void Chunk::debugDump() const {
	cerr << "\t\tsize = " << size() << endl;
	cerr << "\t\tbeg = " << _begin << endl;
	cerr << "\t\tend = " << _end << endl;
	cerr << "\t\taddr = " << this << endl;
	cerr << "\t\tprev = " << _previous << endl;
	cerr << "\t\tnext = " << _next << endl;
	cerr << "\t\ttext = ";
	for(size_t i = 0; i < size(); i++)
		cerr << (char) get(i);
	cerr << endl;
}

