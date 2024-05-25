#pragma once

#include "lve_window.h"
#include "lve_pipeline.h"

namespace lve {
	class App {
	private:
		LveWindow lveWindow{ WIDTH, HEIGHT, "Vulkan Renderer" };
		LvePipeline lvePipeline{"shaders/shader.vert", "shaders/shader.frag"};
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 800;

		void run();
	};
}