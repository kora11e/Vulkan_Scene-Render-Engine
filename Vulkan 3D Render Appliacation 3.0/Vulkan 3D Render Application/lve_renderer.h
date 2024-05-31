#pragma once

#pragma once

#include "lve_window.h"
#include "lve_device.h"
#include "Swapchain.h"

#include <stdexcept>
#include <memory>
#include <vector>
#include <cassert>

namespace lve {
	class LveRenderer {
	public:
		LveRenderer(LveWindow& window, MyEngineDevice& device);
		~LveRenderer();

		LveRenderer(const LveWindow&) = delete;
		LveRenderer& operator=(const LveWindow&) = delete;

		VkRenderPass getSwapChainRenderPass() const { return lveSwapChain->getRenderPass(); };
		float getAspectRatio() const { return lveSwapChain->extentAspectRatio(); };
		bool frameInProgress() { return frameStarted; };

		VkCommandBuffer getCurrentCommandBuffer() const { 
			assert(frameStarted && "Cannot get command buffer when frame not in progress!");
			return commandBuffer[currentImageIndex]; 
		};

		int getFrameIndex() const {
			assert(frameStarted && "Cannot get frame index when frame is not in progress");
			return currentFrameIndex;
		};

		VkCommandBuffer beginFrame();
		void endFrame();
		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

	private:
		void createcommadBuffers();
		void freeCommandBuffers();
		void recreateSwapChain();

		LveWindow& lveWindow;
		MyEngineDevice& lveDevice;
		std::unique_ptr<MyEngineSwapChain> lveSwapChain;
		std::vector<VkCommandBuffer> commandBuffer;
		
		uint32_t currentImageIndex;
		int currentFrameIndex{0};
		bool frameStarted{false};
	};
}