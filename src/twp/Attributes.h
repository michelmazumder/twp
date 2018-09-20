#pragma once

#include "tdm/AttributedRope.h"

namespace twp {
	class Underline : public tdm::Attribute {
	public:
		static const int TYPE_ID = 1;
		Underline(const tdm::AttributedRope *r);
	};

	class Bold : public tdm::Attribute {
	public:
		static const int TYPE_ID = 2;
		Bold(const tdm::AttributedRope *r);
	};
}