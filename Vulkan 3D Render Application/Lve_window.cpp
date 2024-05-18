#include "lve_window.h"

namespace lve {

	LveWindow::LveWindow(int width, int height) : width{ width }, height{ height } {
		initWindow();
	}

	LveWindow::~LveWindow() {
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void LveWindow::initWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		window = glfwCreateWindow(width, height, "Vulkan Application", nullptr, nullptr);

		glfwSetWindowUserPointer(window, this);
		//glfwSetFramebufferSizeCallback(Window, framebufferResizeCallback);
	}
}