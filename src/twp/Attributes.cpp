#include "twp/Attributes.h"
#include <ncurses.h>

using namespace twp;

Underline::Underline(const tdm::AttributedRope *r) :
	tdm::Attribute(r, TYPE_ID, A_UNDERLINE)
{}