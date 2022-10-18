#include "input_manager.h"

InputManager::InputManager()
{
	InputManager(nullptr);
}

InputManager::InputManager(GLFWwindow* window)
	: _window{window}, _scroll_xoffset{0}, _scroll_yoffset{0}
{
}

void InputManager::set_target_window(GLFWwindow* window)
{
	_window = window;
}

void InputManager::process_input()
{
	if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(_window, true);
	}
}

void InputManager::process_scroll_callback(double xoffset, double yoffset)
{
	_scroll_xoffset = xoffset;
	_scroll_yoffset = yoffset;
}

int InputManager::get_scroll_x()
{
	int x_value = 0;

	if (_scroll_xoffset > 0)
		x_value = 1;
	else if (_scroll_xoffset < 0)
		x_value = -1;

	reset_scroll_offset_x();

	return x_value;
}

int InputManager::get_scroll_y()
{
	int y_value = 0;

	if (_scroll_yoffset > 0)
		y_value = 1;
	else if (_scroll_yoffset < 0)
		y_value = -1;

	reset_scroll_offset_y();

	return y_value;
}

void InputManager::reset_scroll_offset_x() { _scroll_xoffset = 0; }
void InputManager::reset_scroll_offset_y() { _scroll_yoffset = 0; }