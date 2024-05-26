#pragma once

#include "lve_window.h"
#include "lve_pipeline.h"
#include "lve_device.h"

namespace lve {
	class App {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void run();
	private:
		LveWindow lveWindow{ WIDTH, HEIGHT, "Vulkan Renderer!"};
		MyEngineDevice lveDevice{lveWindow};
		LvePipeline lvePipeline{lveDevice, "./shader.vert.spv", "./shader.frag.spv", LvePipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
	};

}