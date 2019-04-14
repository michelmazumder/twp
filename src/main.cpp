#include "TwpApplication.h"
#include <panel.h>

int main(int argc, char *argv[]) {

	TwpApplication app;
	app.start();
	app.join();


	/*
	TwpApplication theApp;
	theApp.start();
	theApp.join();
	*/
	return 0;
}