#pragma once

#include <string.h>
#include <iostream>
#include "Semaforo.h"
#include <atomic>

namespace m3d
{
	struct ThreadExecutionResult
	{
		bool error;
		std::string errorMessage;

		void resultOk() { error = false; }
		void resultError(const std::string &err) { error = true; errorMessage = err; }
	};

	std::ostream& operator<<(std::ostream& os, const ThreadExecutionResult& tr);

	class Thread
	{
		friend void *threadFunction(void *callingThread);

		pthread_t _threadDescriptor;

	protected:
		virtual void theJob() = 0;
		ThreadExecutionResult _result;
		std::atomic_bool _runStatus;
		std::string _threadName;

	public:
		Thread(const std::string &threadName);
		
		inline const std::string &name() const
		{
			return _threadName;
		}
		virtual ~Thread();
		// true se riesce a creare il thread, false altrimenti
		bool start();
		ThreadExecutionResult join();
		void requestStop();
	};

	class ThreadDeleter
	{
		Thread *_theThread;

	public:
		ThreadDeleter(Thread *theThread);
	};
}
