#pragma once

#include <string>
#include "win/Window.h"
#include "m3d/Thread.h"
#include "util/VersionDataObject.h"

#ifndef CTRL
#define CTRL(c) ((c) & 037)
#endif


namespace win {

	class Application : public m3d::Thread, win::KeyListener {

	protected:
		util::VersionDataObject version;
		Window *mainWindow; // the view
		std::string appName;

		virtual void theJob();
		virtual Window *createMainWindow();
		virtual void idleCicle();


	public:
		virtual void onKeyPressed(int key) = 0;
		inline Window *getWindow() const { return mainWindow; }

		Application(const std::string& appName, const util::VersionDataObject& version);
		Application(const std::string& appName, const util::VersionDataObject& version, const win::Size& mainWinSize);
		virtual ~Application();
	};
}