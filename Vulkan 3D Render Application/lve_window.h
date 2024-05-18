#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace lve {

	class LveWindow {
	public:
		LveWindow(int w, int h);
		~LveWindow();
	private:
		void initWindow();
		const int width;
		const int height;

		GLFWwindow* window;
	};
}