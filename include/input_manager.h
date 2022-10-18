#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "GLFW/glfw3.h"

class InputManager
{
public:
	InputManager();
	InputManager(GLFWwindow* window);

	void set_target_window(GLFWwindow* window);

	void process_input();
	void process_scroll_callback(double xoffset, double yoffset);

	int get_scroll_x();
	int get_scroll_y();

private:
	GLFWwindow* _window;
	double _scroll_xoffset;
	double _scroll_yoffset;

	void reset_scroll_offset_x();
	void reset_scroll_offset_y();
};

#endif