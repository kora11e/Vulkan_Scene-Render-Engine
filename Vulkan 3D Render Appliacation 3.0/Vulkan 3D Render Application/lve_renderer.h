#pragma once

#pragma once

#include "lve_window.h"
#include "lve_device.h"
#include "Swapchain.h"

#include <stdexcept>
#include <memory>
#include <vector>

namespace lve {
	class Renderer {
	public:
		Renderer(LveWindow& window, Renderer& device);
		~Renderer();

		Renderer(const LveWindow&) = delete;
		Renderer& operator=(const LveWindow&) = delete;

		VkRenderPass getSwapChainRenderPass() const { return lveSwapChain->getRenderPass(); };
		bool frameInProgress() { return frameStarted; };

		VkCommandBuffer getCurrentCommandBuffer() const { 
			assert(frameStarted && "Cannot get command buffer when frame not in progress!");
			return commandBuffer[currentImageIndex]; 
		};

		VkCommandBuffer beginFrame();
		void endFrame();
		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

	private:
		void createcommadBuffers();
		void freeCommandBuffers();
		void recreateSwapChain();

		LveWindow& lveWindow{ WIDTH, HEIGHT, "Vulkan Renderer!" };
		MyEngineDevice& lveDevice{ lveWindow };
		std::unique_ptr<MyEngineSwapChain> lveSwapChain;
		std::vector<VkCommandBuffer> commandBuffer;
		
		uint32_t currentImageIndex;
		bool frameStarted;
	};
}