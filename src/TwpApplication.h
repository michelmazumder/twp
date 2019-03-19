#pragma once

#include "win/Application.h"
#include "twp/Document.h"


struct TwpStatusBar {
	bool underline;
	bool bold;
	int currentPos;
};

class TwpApplication : public win::Application {

	private:
		twp::Document document; // the model
		win::Window *statusWindow;
		TwpStatusBar _statusBar;
		
	
	protected:
		// the view
		virtual win::Window *createMainWindow();
		virtual void idleCicle();
		virtual void onInitComplete();

	public:
		inline TwpStatusBar& statusBar() { return _statusBar; }

		TwpApplication();
		virtual ~TwpApplication();
		void status(const std::string &msg);
		void refreshStatus();

		// caratteri speciali?
		bool isNormalChar(int c) { return true; }

		virtual void onKeyPressed(int key);
};

class DoppiaFinestra : public win::Application {
	private:
		twp::Document document; // the model
	
	protected:
		// the view
		virtual win::Window *createMainWindow();
		virtual void idleCicle();
		virtual void onInitComplete();

	public:
		DoppiaFinestra();

		// caratteri speciali?
		bool isNormalChar(int c) { return true; }

		virtual void onKeyPressed(int key);
};
