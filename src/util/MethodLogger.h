#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <sstream>
#include <memory>
#include <map>
#include <pthread.h>


namespace util
{
	class LogDataObject
	{
	public:
		enum Level { STACK, DEBUG, INFO, WARNING, ERROR };

	protected:
		std::shared_ptr<std::ostringstream> _messageStreamPointer;

		Level _level;
		std::string _timestamp;
		std::string _prettyFunction;
		std::shared_ptr<std::list<std::string>> _callStack;
		std::string _errorUUID;

		const char *level() const;

	public:
		LogDataObject(const LogDataObject &b);

		LogDataObject(Level level, 
			const std::string &timestamp, 
			const std::string &prettyFunction,
			const std::list<std::string> &callStack
		);

		LogDataObject(Level level, 
			const std::string &timestamp, 
			const std::string &prettyFunction,
			const std::list<std::string> &callStack,
			const std::string &errorUUID
		);

		virtual ~LogDataObject();

		template<typename T>
		LogDataObject& operator<< (T val)
		{
			(*_messageStreamPointer) << val;
			return (*this);
		}
	};

	class MethodLogger
	{
		std::string prettyFunction;
		bool isEnabled;
		std::shared_ptr<LogDataObject> theLog;
		
		static std::map<pthread_t, std::list<std::string>> callStack;
		
		std::string timestamp() const;

	public:
		MethodLogger(const char *prettyFunction, bool enabled = true);
		virtual ~MethodLogger();	

		LogDataObject log() const;
		LogDataObject debug() const;
		LogDataObject warning() const;
		LogDataObject error(const char *uuid) const;
		void printStackTrace();
	};
}
