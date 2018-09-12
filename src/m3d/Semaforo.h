#pragma once

#include <semaphore.h>
#include <string>
#include <errno.h>

namespace m3d
{
	class Semaforo
	{
	    sem_t _theSem;
	    std::string _name;

	public:
	    Semaforo(const char * name, int count = 1);
	    ~Semaforo();

	    void lock();
	    void unlock();
	    bool isLocked() const;
	};
}
