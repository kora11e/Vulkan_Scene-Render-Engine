#pragma once

#include "Lve_window.h"

namespace lve {
	class HelloApplication {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void run() {};
	private:
		LveWindow lveWindow{WIDTH, HEIGHT};
	};
}