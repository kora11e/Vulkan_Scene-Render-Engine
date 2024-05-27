#include "lve_window.h"
#include <stdexcept>

namespace lve {

	LveWindow::LveWindow(int w, int h, std::string name) : width{ w }, height{ h }, windowName{ name } {
		initWindow();
	};

	LveWindow::~LveWindow() {
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void LveWindow::initWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		window = glfwCreateWindow(width, height, "Vulkan Application", nullptr, nullptr);

		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, frameBufferResizedCallback);
		//glfwSetFramebufferSizeCallback(Window, framebufferResizeCallback);
	}
	void LveWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create a window.");
		}
	};

	void LveWindow::frameBufferResizedCallback(GLFWwindow* window, int width, int height) {
		auto lveWindow = reinterpret_cast<LveWindow*>(glfwGetWindowUserPointer(window));
		lveWindow->frameBufferResized = true;
		lveWindow->width = width;
		lveWindow->height = height;
	};
}