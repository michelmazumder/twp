#pragma once

#include "win/Application.h"
#include "twp/Document.h"



class TwpApplication : public win::Application {

	private:
		twp::Document document; // the model
	
	protected:
		// the view
		virtual win::Window *createMainWindow();
		virtual void idleCicle();
		virtual void onInitComplete();

	public:
		TwpApplication();

		// caratteri speciali?
		bool isNormalChar(int c) { return true; }

		virtual void onKeyPressed(int key);
};

