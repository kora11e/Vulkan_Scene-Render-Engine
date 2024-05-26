#pragma once

#include "lve_window.h"
#include "lve_pipeline.h"
#include "lve_device.h"
#include "Swapchain.h"
#include <memory>
#include <vector>

namespace lve {
	class App {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		App();
		~App();

		App(const LveWindow&) = delete;
		App& operator=(const LveWindow&) = delete;

		void run();
	private:
		void createPipelineLayout();
		void createPipeline();
		void createcommadBuffers();
		void drawFrame();

		LveWindow lveWindow{ WIDTH, HEIGHT, "Vulkan Renderer!"};
		MyEngineDevice lveDevice{lveWindow};
		MyEngineSwapChain lveSwapChain{lveDevice, lveWindow.getExtent()};
		std::unique_ptr<LvePipeline> lvePipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffer;

	};

}