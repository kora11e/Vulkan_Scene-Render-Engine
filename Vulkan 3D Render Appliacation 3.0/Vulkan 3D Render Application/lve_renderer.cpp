#include "lve_renderer.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <array>
#include <cassert>

namespace lve {
	
	LveRenderer::LveRenderer(LveWindow& window, MyEngineDevice& device) : lveWindow{ window }, lveDevice{ device } {
		recreateSwapChain();
		createcommadBuffers();
		
	}

	LveRenderer::~LveRenderer() {
		freeCommandBuffers();
	}

	void LveRenderer::recreateSwapChain() {
		auto extent = lveWindow.getExtent();
		while (extent.width == 0 || extent.height == 0) {
			extent = lveWindow.getExtent();
			glfwWaitEvents();
		}
		vkDeviceWaitIdle(lveDevice.device());

		if (lveSwapChain == nullptr) {
			lveSwapChain = std::make_unique<MyEngineSwapChain>(lveDevice, extent);
		} else {
			std::shared_ptr<MyEngineSwapChain> oldSwapChain = std::move(lveSwapChain);
			lveSwapChain = std::make_unique<MyEngineSwapChain>(lveDevice, extent, oldSwapChain);
			
			if (oldSwapChain->compareSwapFormats(*lveSwapChain.get())) {
				throw std::runtime_error("Swap chain image(or depth) format has changed!");
			}
		}
		//add stuff later here
	}

	void LveRenderer::freeCommandBuffers() {
		vkFreeCommandBuffers(lveDevice.device(), lveDevice.getCommandPool(), static_cast<uint32_t>(commandBuffer.size()), commandBuffer.data());
		commandBuffer.clear();
	}

	void LveRenderer::createcommadBuffers() {
		commandBuffer.resize(MyEngineSwapChain::MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo alloInfo{};
		alloInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		alloInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		alloInfo.commandPool = lveDevice.getCommandPool();
		alloInfo.commandBufferCount = static_cast<uint32_t>(commandBuffer.size());

		if (vkAllocateCommandBuffers(lveDevice.device(), &alloInfo, commandBuffer.data()) != VK_SUCCESS) {
			throw std::runtime_error("Failed to allocate command buffers!");
		}

	}

	VkCommandBuffer LveRenderer::beginFrame() {
		
		assert(!frameStarted && "Can't call beginFrame!");

		auto result = lveSwapChain->acquireNextImage(&currentImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapChain();
			return nullptr;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("Failed to acquire swap chain image!");
		}

		frameStarted = true;

		auto commandBuffer = getCurrentCommandBuffer();

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("Failed to begin recording command buffer!");
		}
		return commandBuffer;
	};
		
	void LveRenderer::endFrame() {
		assert(frameStarted && "Can't call endFrame while frame is not in progress!");

		auto commandBuffer = getCurrentCommandBuffer();
		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
			throw std::runtime_error("Failed to record command buffer!");
		}
		auto result = lveSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || lveWindow.windowResized()) {
			lveWindow.resetWindowResizedFlag();
			recreateSwapChain();
			return;
		} else if (result != VK_SUCCESS) {
			throw std::runtime_error("Failed to present swap chain image!");
		}
		frameStarted = false;
		currentFrameIndex = (currentFrameIndex + 1) % MyEngineSwapChain::MAX_FRAMES_IN_FLIGHT;
	};

	void LveRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer) {
		assert(frameStarted && "Can't call beginSwapChainRenderPass");
		assert(commandBuffer == getCurrentCommandBuffer() && "Can't begin render pass on command buffer from a different frame");

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = lveSwapChain->getRenderPass();
		renderPassInfo.framebuffer = lveSwapChain->getFrameBuffer(currentImageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = lveSwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValue{};
		clearValue[0].color = { 0.01f, 0.01f, 0.01f, 1.f };
		clearValue[1].depthStencil = { 1.0f, 0 };         //check that later cuz something is wrong with number format
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValue.size());
		renderPassInfo.pClearValues = clearValue.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(lveSwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(lveSwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, lveSwapChain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	};
	void LveRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer) {
		assert(frameStarted && "Can't call endSwapChainRenderPass if frame is not in progress");
		assert(commandBuffer == getCurrentCommandBuffer() && "Can't end render pass on command buffer from a different frame");

		vkCmdEndRenderPass(commandBuffer);
	};
	
}