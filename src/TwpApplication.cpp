#include "TwpApplication.h"
#include "util/MethodLogger.h"
#include "win/Screen.h"
#include "win/BorderedWindow.h"

TwpApplication::TwpApplication()
	: win::Application("twp", util::VersionDataObject("0.1.1")) 
{}


win::Window *TwpApplication::createMainWindow() {
	util::MethodLogger m(__PRETTY_FUNCTION__);
	// win::Size sz(win::getScreenHeight() - 1, win::getScreenWidth());
	win::Size sz(win::getScreenHeight() / 2, win::getScreenWidth() / 2);
	m.log() << "win size => w:" << sz.width << " h:" << sz.height;
	win::Point initPoint(win::getScreenHeight() / 4, win::getScreenWidth() / 4);
	m.log() << "init point => r:" << initPoint.row << " c:" << initPoint.col;
	return new win::BorderedWindow(initPoint, sz);
}

void TwpApplication::onInitComplete() {
	util::MethodLogger m(__PRETTY_FUNCTION__);
	mainWindow->repaint();
	document.render(*mainWindow);
	refresh();
}

void TwpApplication::onKeyPressed(int key) {
	util::MethodLogger m(__PRETTY_FUNCTION__);
	m.log() << "||| Key pressed = " << keyname(key) << " . " << key;
	//m.log() << "bksp = " << KEY_BACKSPACE;
	//m.log() << "dl = " << KEY_DL;
	//document.debugDump();

	switch(key) {
		case CTRL('r'):
			// for document rendering
			break;
		case CTRL('d'):
			document.debugDump(std::cerr);
			break;
		case CTRL('q'):
			m.log() << "REQUEST STOP!";
			requestStop();
			return;
		case KEY_LEFT:
			document.left();
			break;
		case KEY_RIGHT:
			document.right();
			break;
		case KEY_DC:
			document.deleteAtCurrentPos();
			break;
		case KEY_HOME:
			document.home();
			break;
		case KEY_END:
			document.end();
			break;
		case KEY_BACKSPACE:
		case 127:
			document.left();
			document.deleteAtCurrentPos();
			break;
		case CTRL('u'):
			document.toggleUnderline();
			break;
		case CTRL('b'):
			document.toggleBold();
			break;

		default:
			if(isNormalChar(key)) {
				m.log() << "is normal key...";
				document.insertChar((char) key);
				m.log() << "char inserted";
			}
	}
	m.log() << "document render";
	document.render(*mainWindow);
}

void TwpApplication::idleCicle() {
	document.defrag();
}

// --------------------------------------------------------
DoppiaFinestra::DoppiaFinestra()
	: win::Application("DoppiaFinestra", util::VersionDataObject("0.1")) 
{}


win::Window *DoppiaFinestra::createMainWindow() {
	util::MethodLogger m(__PRETTY_FUNCTION__);
	// win::Size sz(win::getScreenHeight() - 1, win::getScreenWidth());
	win::Size sz(win::getScreenHeight() - 5, win::getScreenWidth());
	m.log() << "win size => w:" << sz.width << " h:" << sz.height;
	win::Point initPoint(0, 0);
	m.log() << "init point => r:" << initPoint.row << " c:" << initPoint.col;
	return new win::BorderedWindow(initPoint, sz);
}

void DoppiaFinestra::onInitComplete() {
	util::MethodLogger m(__PRETTY_FUNCTION__);
	mainWindow->repaint();
	document.render(*mainWindow);
	refresh();
	wrefresh(stdscr);
}

void DoppiaFinestra::onKeyPressed(int key) {
	util::MethodLogger m(__PRETTY_FUNCTION__);
	m.log() << "||| Key pressed = " << keyname(key) << " . " << key;
	//m.log() << "bksp = " << KEY_BACKSPACE;
	//m.log() << "dl = " << KEY_DL;
	//document.debugDump();

	switch(key) {
		case CTRL('r'):
			// for document rendering
			break;
		case CTRL('d'):
			document.debugDump(std::cerr);
			break;
		case CTRL('q'):
			m.log() << "REQUEST STOP!";
			requestStop();
			return;
		case KEY_LEFT:
			document.left();
			break;
		case KEY_RIGHT:
			document.right();
			break;
		case KEY_DC:
			document.deleteAtCurrentPos();
			break;
		case KEY_HOME:
			document.home();
			break;
		case KEY_END:
			document.end();
			break;
		case KEY_BACKSPACE:
		case 127:
			document.left();
			document.deleteAtCurrentPos();
			break;
		case CTRL('u'):
			document.toggleUnderline();
			break;
		case CTRL('b'):
			document.toggleBold();
			break;

		default:
			if(isNormalChar(key)) {
				m.log() << "is normal key...";
				document.insertChar((char) key);
				m.log() << "char inserted";
			}
	}
	m.log() << "document render";
	document.render(*mainWindow);
}

void DoppiaFinestra::idleCicle() {
	document.defrag();
}
