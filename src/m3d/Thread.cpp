#include <pthread.h>
#include "m3d/Thread.h"
#include "util/MethodLogger.h"

using namespace std;

namespace m3d
{
	void *threadFunction(void *callingThread)
	{
		util::MethodLogger m(__PRETTY_FUNCTION__);
		Thread *ct = reinterpret_cast<Thread *>(callingThread);
		m.log() << "RTTI: " << typeid(*ct).name();
		m.log() << "Starting thread: " << ct->name();
		ct->theJob();
		m.log() << "Ending thread: " << ct->name();
		return nullptr;
	}


	void *deleterFunction(void *threadToDelete)
	{
		Thread *deleteMe = reinterpret_cast<Thread *>(threadToDelete);
		deleteMe->join();
		delete deleteMe;	
		return nullptr;
	}


	ThreadDeleter::ThreadDeleter(Thread *theThread) : _theThread(theThread)
	{
		util::MethodLogger m(__PRETTY_FUNCTION__);
		pthread_t descr;
		if(pthread_create(&descr, nullptr, deleterFunction, theThread) != 0)
		{
			util::MethodLogger m(__PRETTY_FUNCTION__);
			m.error("0bb5169d-df0d-461a-a881-4dad86617ebb") << "Errore creazione deleter thread!";
		}
		m.log() << "Creato thread con id = " << descr;
	}

	Thread::Thread(const std::string& threadName) : 
		_threadName(threadName)
	{
		util::MethodLogger m(__PRETTY_FUNCTION__);
		m.log() << "Creo nuovo thread: " << name();	
	}

	Thread::~Thread()
	{
		util::MethodLogger m(__PRETTY_FUNCTION__);
		join();
		m.error("ad566ddd-2125-4c52-99e4-a6184472799b") << "Delete di Thread: " << name();
	}

	void Thread::requestStop() 
	{
		util::MethodLogger m(__PRETTY_FUNCTION__);
		m.log() << "Thread " << this->name() << ": è stata fatta una requestStop";
		m.printStackTrace();
		_runStatus = false;
	}

	bool Thread::start()
	{
		util::MethodLogger m(__PRETTY_FUNCTION__);
		_runStatus = true;
		m.log() << "Partenza thread: " << this->name();
		int res = pthread_create(&_threadDescriptor, NULL, threadFunction, this);
		if(res != 0)
		{
			m.error("5eb0ffd0-7486-4822-8c1a-0bbd53ecc08c") << "Errore creazione thread!";
			return false;
		}
		else
		{
			m.log() << "Creazione thread ok";
		}
		return true;
	}

	ThreadExecutionResult Thread::join()
	{
		util::MethodLogger m(__PRETTY_FUNCTION__);
		m.log() << "Joining thread... " << this->name();
		pthread_join(_threadDescriptor, NULL);
		m.log() << this->name() << " joined!";
		return _result;
	}

	ostream& operator<<(ostream& os, const ThreadExecutionResult& tr)
	{
		if(tr.error)
		{
			os << "thread error: " << tr.errorMessage;
		}
		else
		{
			os << "no error, executiong ok.";
		}
	    return os;
	}
}
