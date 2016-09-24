#ifndef _INPUT_H_
#define _INPUT_H_

#include <GLFW/glfw3.h>
#include <vector>
#include <memory>
#include <map>

#include "Delegate.h"

#define GLFW_RELEASED -1

using std::unique_ptr;
using std::make_unique;

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

	// Well this is really nice TODO: Implement this unique_ptr's into the project..right?...jesus
	template <typename T>
	using vectorPtr = unique_ptr<vector<T>>;

	typedef map<const int, const unique_ptr<KeyState>> KeyStateMap;
	typedef unique_ptr<map<const int, const unique_ptr<KeyState>>> KeyStateMapPtr;

	static void init();

	static int getKey(const int &key);
	static int getKey(const int &key, const int &mods);

	static const Position & getCursorPosition();
	static Position deltaCursorPosition();

	static int getMouseButton(const int &button);
	static int getMouseButton(const int &button, const int &mods);

	static Position getScrollPosition();

	static void addOnKeyCallback(const OnKeyCallback &delegate);
	static void addOnCursorPosCallback(const OnCursorPosCallback &delegate);
	static void addOnMouseButtonCallback(const OnMouseButtonCallback &delegate);
	static void addOnScrollCallback(const OnScrollCallback &delegate);
	static void addOnCursorEnterCallback(const OnCursorEnterCallback &delegate);

	static void lateUpdate();

	static void quit();
private:

	static void onKey(GLFWwindow *window, int key, int scanecode, int action, int mods);
	static void onCursorPos(GLFWwindow *window, double x, double y);
	static void onMouseButton(GLFWwindow *window, int button, int action, int mods);
	static void onScroll(GLFWwindow *window, double x, double y);
	static void onCursorEnter(GLFWwindow *window, int state);

	static vector<OnKeyCallback> *const m_onKeyCallbacks;
	static const KeyStateMapPtr m_keyStates;

	static vector<OnCursorPosCallback> *const m_onCursorPosCallback;
	static Position *const m_cursorPos;
	static Position *const m_prevCursorPos;

	static vector<OnMouseButtonCallback> *const m_onMouseButtonCallbacks;
	static const KeyStateMapPtr m_mouseButtonStates;

	static vector<OnScrollCallback> *const m_onScrollCallback;
	static Position *const m_scrollPos;
	static Position *const m_prevScrollPos;

	static vector<OnCursorEnterCallback> *const m_onCursorEnterCallback;
};

#endif // INPUT_H_