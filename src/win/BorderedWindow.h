#pragma once

#include "win/Window.h"

namespace win {
	
	class BorderedWindow : public Window {

	protected:
		Window contentPane;

	public:
		BorderedWindow();
		BorderedWindow(const Point& startingPoint, const Size& size);

		virtual void print(const std::string& s, const Point& relativePosition);
		virtual void print(const std::string& s);
		virtual void moveCursorTo(const Point& p);
		virtual void activateBlinking();
		virtual void deactivateBlinking();
		virtual void attributeOn(int value);
		virtual void attributeOff(int value);

		virtual void repaint();
	};

}