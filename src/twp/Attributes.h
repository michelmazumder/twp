#pragma once

#include "tdm/AttributedRope.h"


namespace twp {
	class Italic : public tdm::Attribute {
	public:
		Italic(const tdm::Cursor& start, const tdm::Cursor& end);
	};
}