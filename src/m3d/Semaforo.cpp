#include "m3d/Semaforo.h"
#include <iostream>
#include <cstring>
#include "util/MethodLogger.h"

namespace m3d
{
    using namespace std;

    Semaforo::Semaforo(const char * name, int count):
    	_name(name)
    {
        sem_init(&_theSem, 0, count);
    }

    void Semaforo::lock()
    {
        util::MethodLogger m(__PRETTY_FUNCTION__, false);
        // cout << "Semaforo " << _name << ": LOCK" << endl;
        int err = sem_wait(&_theSem);
        if(err == -1)
        {
            m.error("f070a3b5-edfe-4ea7-8719-12ebdd3e579b") << "Errore lock semaforo " << _name << ": " << errno 
                << ": " << strerror(errno);
            switch(errno)
            {
                case EINTR: m.log() << "The call was interrupted by a signal handler"; break;
                case EINVAL: m.log() << "sem is not a valid semaphore."; break;
            }
        }
    }

    void Semaforo::unlock()
    {
        util::MethodLogger m(__PRETTY_FUNCTION__, false);
        // cout << "Semaforo " << _name << ": UNLOCK" << endl;
        int err = sem_post(&_theSem);
        if(err == -1)
        {
            cout << "Errore unlock semaforo " << _name << ": " << errno << ": "
                << strerror(errno);
            switch(errno)
            {
                case EOVERFLOW: m.log() << "The maximum allowable value for a semaphore would be exceeded."; break;
                case EINVAL: m.log() << "sem is not a valid semaphore."; break;
            }
        }
    }

    Semaforo::~Semaforo()
    {
        sem_destroy(&_theSem);
    }

    bool Semaforo::isLocked() const
    {
        util::MethodLogger m(__PRETTY_FUNCTION__, false);
        int value = -99;
        if(sem_getvalue(const_cast<sem_t *>(&_theSem), &value) == -1)
        {
            m.error("341e68f8-cb28-476c-8207-e7f50e510d4c") 
                << "Errore in Semaforo::isLocked()." 
                << errno << ": " << strerror(errno);
            switch(errno)
            {
                case EINVAL: 
                    m.log() << "sem is not a valid semaphore << "; 
                    break;
            }
            return false;

        }
        return (value < 1);
    }
}
