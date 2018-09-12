#pragma once

#include <ncurses.h>
#include <string>

namespace win {
	class Window;
	
	class KeyListener {
	public:
		virtual void onKeyPressed(int key) = 0;
	};

	class RenderAble {
	public:
		virtual void render(Window& win) = 0;

	};

	struct Size {
		int height;
		int width;

		Size(int h, int w) : height(h), width(w) {}
		Size(const Size& b) : height(b.height), width(b.width) {}

		Size& operator= (const Size& b) {
			height = b.height;
			width = b.width;
			return *this;
		}
	};

	struct Point {
		int row; // y
		int col; // x

		Point(int r, int c) : row(r), col(c) {}
		Point(const Point& p) : row(p.row), col(p.col) {}

		Point& operator= (const Point& p) {
			row = p.row;
			col = p.col;
			return *this;
		}
	};

	class Window {
	protected:
		WINDOW *ncursesWindow;
		Size size;
		Point upperLeft;

	public:

		Window();
		Window(const Point& startingPoint, const Size& size);
		virtual ~Window();
		
		virtual void print(const std::string& s, const Point& relativePosition);
		virtual void print(const std::string& s);
		virtual void repaint();
		virtual void moveWindowTo(const Point& p);
		virtual void moveCursorTo(const Point& p);
		virtual void activateBlinking();
		virtual void deactivateBlinking();

		inline const Size& getSize() const { return size; }
	};
}