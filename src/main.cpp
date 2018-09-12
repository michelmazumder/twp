#include "TwpApplication.h"

int main(int argc, char *argv[]) {
	TwpApplication theApp;
	theApp.start();
	theApp.join();
	return 0;
}