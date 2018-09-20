#pragma once

#include <iostream>

namespace util {
	class Debuggable {
		virtual std::ostream& debugDump(std::ostream& debugStream) const = 0;
	};
}