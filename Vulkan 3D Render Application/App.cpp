#include "App.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

namespace lve {
	void App::run() {
		while (!lve::LveWindow.shouldClose()) {
			glfwPollEvents();
		}
	}
}