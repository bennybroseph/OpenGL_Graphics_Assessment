#ifndef _INPUT_H_
#define _INPUT_H_

#include <GLFW/glfw3.h>
#include <vector>
#include <map>

#include "Delegate.h"

#define GLFW_RELEASED -1

using std::vector;
using std::map;

typedef Delegate<void, int, int, int, int> OnKeyCallback;
typedef Delegate<void, double, double> OnCursorPosCallback;
typedef Delegate<void, int, int, int> OnMouseButtonCallback;
typedef Delegate<void, double, double> OnScrollCallback;
typedef Delegate<void, int> OnCursorEnterCallback;

//TODO: Convert to namespace. Static class seems unnecessary without a singleton pattern
class Input
{
public:
	struct KeyState
	{
		int state = GLFW_RELEASED;
		int mods = 0x0;
	};
	struct Position
	{
		double x = 0;
		double y = 0;

		Position operator-(const Position& other) const
		{
			Position difference;
			difference.x = this->x - other.x;
			difference.y = this->y - other.y;

			return difference;
		}
	};

	static void init();

	static int getKey(const int &key);
	static int getKey(const int &key, const int &mods);

	static const Position& getCursorPosition();
	static Position deltaCursorPosition();

	static int getMouseButton(const int &button);
	static int getMouseButton(const int &button, const int &mods);

	static const Position& getScrollPosition();

	static void addOnKeyCallback(const OnKeyCallback &delegate);
	static void addOnCursorPosCallback(const OnCursorPosCallback &delegate);
	static void onMouseButtonCallback(const OnMouseButtonCallback &delegate);
	static void onScrollCallback(const OnScrollCallback &delegate);
	static void onCursorEnterCallback(const OnCursorEnterCallback &delegate);

	static void lateUpdate();

	static void quit();
	static Position m_cursorPos;
	static Position m_prevCursorPos;
private:

	static void onKey(GLFWwindow *window, int key, int scanecode, int action, int mods);
	static void onCursorPos(GLFWwindow *window, double x, double y);
	static void onMouseButton(GLFWwindow *window, int button, int action, int mods);
	static void onScroll(GLFWwindow *window, double x, double y);
	static void onCursorEnter(GLFWwindow *window, int state);

	static vector<OnKeyCallback> m_onKeyCallbacks;
	static map<int, KeyState> m_keyStates;

	static vector<OnCursorPosCallback> m_onCursorPosCallback;

	static vector<OnMouseButtonCallback> m_onMouseButtonCallbacks;
	static map<int, KeyState> m_mouseButtonStates;

	static vector<OnScrollCallback> m_onScrollCallback;
	static Position m_scrollPos;
	static Position m_prevScrollPos;

	static vector<OnCursorEnterCallback> m_onCursorEnterCallback;
};

#endif // INPUT_H_