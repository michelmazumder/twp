#pragma once
#include "m3d/Semaforo.h"

namespace m3d {
	class Lock
	{
	    Semaforo& _theLock;

	public:
	    Lock(Semaforo& theLock);
	    ~Lock();
	};
}
