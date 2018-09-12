#include "m3d/Lock.h"

namespace m3d
{
	Lock::Lock(Semaforo& theLock) : _theLock(theLock)
	{
	    _theLock.lock();
	}

	Lock::~Lock()
	{
		_theLock.unlock();
	}
}
