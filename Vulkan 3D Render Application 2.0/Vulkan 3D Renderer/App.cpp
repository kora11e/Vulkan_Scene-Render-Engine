#include "App.h"

namespace lve {
	void App::run() {
		while (lveWindow.shouldClose()) {
			glfwPollEvents();
		}
	}
}