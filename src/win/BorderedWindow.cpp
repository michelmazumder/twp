#include "win/BorderedWindow.h"
#include "win/Screen.h"
#include "util/MethodLogger.h"

using namespace win;

BorderedWindow::BorderedWindow() : Window(), contentPane(Point(1,1), Size(getScreenHeight() - 2, getScreenWidth() - 2)) {
	util::MethodLogger m(__PRETTY_FUNCTION__);
}

BorderedWindow::BorderedWindow(const Point& startingPoint, const Size& size) 
	: Window(startingPoint, size), contentPane(Point(startingPoint.row + 1, startingPoint.col + 1),
		Size(Size(size.height-2, size.width-2)))
{
	util::MethodLogger m(__PRETTY_FUNCTION__);
}

void BorderedWindow::repaint() {
	if(ncursesWindow != 0) {
		util::MethodLogger m(__PRETTY_FUNCTION__);
		m.log() << "Disegno BORDO";
		box(ncursesWindow,0,0);
		wrefresh(ncursesWindow);
	}
	contentPane.repaint();
}

void BorderedWindow::print(const std::string& s, const Point& relativePosition) {
	contentPane.print(s, relativePosition);
}

void BorderedWindow::attributeOn(int value) {
	contentPane.attributeOn(value);
}

void BorderedWindow::attributeOff(int value) {
	contentPane.attributeOff(value);
}

void BorderedWindow::print(const std::string& s) {
	contentPane.print(s);
}

void BorderedWindow::moveCursorTo(const Point& p) {
	contentPane.moveCursorTo(p);
}

void BorderedWindow::activateBlinking() {
	contentPane.activateBlinking();
}

void BorderedWindow::deactivateBlinking() {
	contentPane.deactivateBlinking();
}

