#include "App.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

namespace lve {
	void App::run() {
		while (!lveWindow.shouldClose()) {
			glfwPollEvents();
		}
	}
}