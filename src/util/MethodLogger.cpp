#include "util/MethodLogger.h"
#include <stdio.h>
#include <sys/time.h>


namespace util
{
	using namespace std;

	map< pthread_t, list<string> > MethodLogger::callStack;

	MethodLogger::MethodLogger(const char *m, bool enabled) : 
		prettyFunction(m), isEnabled(enabled)
	{
		callStack[pthread_self()].push_front(m);
		if(isEnabled) 
		{
			LogDataObject(LogDataObject::STACK, timestamp(), prettyFunction, callStack[pthread_self()]) 
				<< "--> Enter '" << prettyFunction << "'";
		}
	}

	MethodLogger::~MethodLogger()
	{
		callStack[pthread_self()].pop_front();
		if(isEnabled)
		{
			LogDataObject(LogDataObject::STACK, timestamp(), prettyFunction, callStack[pthread_self()])
				<< "<-- Exit '" << prettyFunction << "'";
		}
	}

	LogDataObject MethodLogger::log() const
	{
		return LogDataObject(LogDataObject::INFO, timestamp(), prettyFunction, callStack[pthread_self()]);
	}

	LogDataObject MethodLogger::debug() const
	{
		return LogDataObject(LogDataObject::DEBUG, timestamp(), prettyFunction, callStack[pthread_self()]);
	}

	LogDataObject MethodLogger::warning() const
	{
		return LogDataObject(LogDataObject::WARNING, timestamp(), prettyFunction, callStack[pthread_self()]);
	}

	LogDataObject MethodLogger::error(const char *uuid) const
	{
		return LogDataObject(LogDataObject::ERROR, timestamp(), prettyFunction, callStack[pthread_self()], uuid);
	}

	std::string MethodLogger::timestamp() const
	{
		char timestamp[255];

		struct timeval detail_time;
		gettimeofday(&detail_time,NULL);

		sprintf(timestamp, "%8d", detail_time.tv_usec);
		return std::string(timestamp);
	}

	void MethodLogger::printStackTrace()
	{
		cerr << "STAMPA STACK TRACE:" << endl;
		string space = " ";
		for(auto s : callStack[pthread_self()])
		{
			cerr << space << s << endl;
			space += " ";
		}
	}

	/*  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  */

	LogDataObject::LogDataObject(const LogDataObject &b):
		_messageStreamPointer(b._messageStreamPointer),
		_level(b._level), 
		_timestamp(b._timestamp),
		_prettyFunction(b._prettyFunction),
		_callStack(b._callStack),
		_errorUUID(b._errorUUID)
	{}

	LogDataObject::LogDataObject(Level level, const std::string &timestamp, const std::string &prettyFunction,
		const std::list<std::string> &callStack) : _level(level), _timestamp(timestamp),
		_prettyFunction(prettyFunction)
	{
		_messageStreamPointer = std::shared_ptr<std::ostringstream>(new std::ostringstream());
		_callStack = std::shared_ptr<std::list<std::string>>(new std::list<std::string>(callStack));
	}

	LogDataObject::LogDataObject(Level level, const std::string &timestamp, const std::string &prettyFunction,
		const std::list<std::string> &callStack, const std::string &errorUUID) : _level(level), _timestamp(timestamp),
		_prettyFunction(prettyFunction), _errorUUID(errorUUID)
	{
		_messageStreamPointer = std::shared_ptr<std::ostringstream>(new std::ostringstream());
		_callStack = std::shared_ptr<std::list<std::string>>(new std::list<std::string>(callStack));
	}

	LogDataObject::~LogDataObject()
	{
		// stampa del log
		cerr
			<< "[LVL: " << level() << "]\t"
			<< "[TID: " << pthread_self() << "]\t"
			<< "[TS: " << _timestamp << "]\t";

		cerr << "[Msg: " << _messageStreamPointer->str() << "]\t";

		if(_level == ERROR)
		{
			cerr << endl << "\t[CALL STACK: " << endl;
			string blanks = " ";
			for(auto s : *_callStack)
			{
				cerr << blanks << s << endl;
				blanks += " ";
			}
			cerr << endl << "\tEND CALL STACK]";
		}

		cerr << endl;
	}

	const char *LogDataObject::level() const
	{
		switch(_level) {
			case STACK: return "STACK";
			case DEBUG: return "DEBUG";
			case INFO: return "INFO";
			case WARNING: return "WARNING";
			case ERROR: return "ERROR";
		}
	}
}
