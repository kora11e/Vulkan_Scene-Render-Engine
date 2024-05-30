#pragma once

#include "lve_window.h"
#include "lve_pipeline.h"
#include "lve_device.h"
#include "gameObject.h"
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
		void loadModels();
		void loadgameObjects();
		void createPipelineLayout();
		void createPipeline();
		void createcommadBuffers();
		void freeCommandBuffers();
		void drawFrame();
		void recreateSwapChain();
		void recordCommandBuffer(int imageIndex);
		void renderGameObjects(VkCommandBuffer commandBuffer);

		LveWindow lveWindow{ WIDTH, HEIGHT, "Vulkan Renderer!"};
		MyEngineDevice lveDevice{lveWindow};
		std::unique_ptr<MyEngineSwapChain> lveSwapChain;
		std::unique_ptr<LvePipeline> lvePipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffer;
		std::vector<LveGameObject> gameObjects;
		std::unique_ptr<LveModel> lveModel;
	};

}