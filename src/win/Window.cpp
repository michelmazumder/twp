#include "win/Window.h"
#include "util/MethodLogger.h"

using namespace win;

Window::Window() : Window(win::Point(0,0), win::Size(0,0)) {
	util::MethodLogger m(__PRETTY_FUNCTION__);	
}

Window::Window(const Point& _startingPoint, const Size& _size) :
	size(_size), upperLeft(_startingPoint)
{
	util::MethodLogger m(__PRETTY_FUNCTION__);
	ncursesWindow = newwin(size.height, size.width, upperLeft.row, upperLeft.col);
	if(ncursesWindow == NULL) {
		m.error("D3423586-870C-4C00-ACC4-04B50CA0A8EB") << "newwin ha restituito NULL";
	}
}

Window::~Window() {
	util::MethodLogger m(__PRETTY_FUNCTION__);	
	if(ncursesWindow != 0) {
		delwin(ncursesWindow);
		ncursesWindow = 0;
	}
}

void Window::moveWindowTo(const Point& p) {
	util::MethodLogger m(__PRETTY_FUNCTION__);	

	upperLeft = p;

	if(ncursesWindow != 0) {
		delwin(ncursesWindow);
		ncursesWindow = 0;
	}

	ncursesWindow = newwin(size.height, size.width, upperLeft.row, upperLeft.col);
	if(ncursesWindow == NULL) {
		m.error("01DD7F38-B683-4C87-BAEC-C56EDECB54DE") << "newwin ha restituito NULL";
	}
	
	repaint();
}

void Window::repaint() {
	if(ncursesWindow != NULL) {
		wrefresh(ncursesWindow);
	}
}

void Window::print(const std::string& s, const Point& relativePosition) {
	if(s.length() == 0) return;
	if(ncursesWindow != NULL) {
		int rt = ::mvwprintw(ncursesWindow, relativePosition.row, relativePosition.col, s.c_str());
	}
	else {
		util::MethodLogger m(__PRETTY_FUNCTION__);
		m.error("AFDEF480-499B-4174-AE4D-1677DA0282C3") << "print failed: window is null";
	}
}

void Window::print(const std::string& s) {
	if(s.length() == 0) return;
	if(ncursesWindow != NULL) {
		int rt = ::wprintw(ncursesWindow, s.c_str());
	}
	else {
		util::MethodLogger m(__PRETTY_FUNCTION__);
		m.error("817193A0-4CF2-4A37-A741-D221EE5BA9C2") << "print failed: window is null";
	}
}

void Window::moveCursorTo(const Point& p) {
	if(ncursesWindow != NULL) {
		wmove(ncursesWindow, p.row, p.col);
	}
}

void Window::activateBlinking() {
	if(ncursesWindow != NULL) {
		wattron(ncursesWindow, A_BLINK); 
		wattron(ncursesWindow, A_REVERSE);
	}
}

void Window::deactivateBlinking() {
	if(ncursesWindow != NULL) {
		wattroff(ncursesWindow, A_BLINK);
		wattroff(ncursesWindow, A_REVERSE);
	}
}
