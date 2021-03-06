#include "win/Application.h"
#include <ncurses.h>
#include "util/MethodLogger.h"
#include "win/BorderedWindow.h"

using namespace win;

Application::Application(const std::string& _appName, const util::VersionDataObject& _version)
	: appName(_appName), version(_version), m3d::Thread(_appName), mainWindow(nullptr) {
	util::MethodLogger m(__PRETTY_FUNCTION__);
	initscr();			/* Start curses mode 		*/
    raw();				/* Line buffering disabled	*/
	keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
	noecho();
	cbreak();	/* Line buffering disabled. pass on everything */
	curs_set(0); // cursor invisible
}


Application::~Application() {
	if(mainWindow != nullptr) {
		delete mainWindow;
		mainWindow = nullptr;
	}
	endwin();			/* End curses mode		  */
}

Window *Application::createMainWindow() {
	return new BorderedWindow();
}

void Application::theJob() {
	util::MethodLogger m(__PRETTY_FUNCTION__);

	if(mainWindow == nullptr) {
		mainWindow = createMainWindow();
	}

	onInitComplete();

	// halfdelay(1); // 1/10 di sec
	halfdelay(3);
	// halfdelay(10); // 1 sec

	while(_runStatus) {
		int c = getch();

		if(c != ERR) {
    		this->onKeyPressed(c);
			if(mainWindow != nullptr) {
				mainWindow->repaint();
			}
		}
		else {
			idleCicle();
		}
	}
}

void Application::idleCicle() {
	// nothing
}