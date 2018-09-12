#include "win/Screen.h"
#include <ncurses.h>

namespace win {

	int getScreenHeight() {
		return LINES;
	}

	int getScreenWidth() {
		return COLS;
	}

}