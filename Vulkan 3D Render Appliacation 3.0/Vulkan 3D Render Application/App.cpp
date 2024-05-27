#include "App.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <array>

namespace lve {

	App::App() {
		createPipeline();
		createPipelineLayout();
		createcommadBuffers();
	}

	App::~App() {
		vkDestroyPipelineLayout(lveDevice.device(), pipelineLayout, nullptr);
	}

	void App::run() {
		while (!lveWindow.shouldClose()) {
		
			glfwPollEvents();
			drawFrame();
		}

		vkDeviceWaitIdle(lveDevice.device());
	}

	void App::loadModels() {
		std::vector<LveModel::Vertex> vertices{
			{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, 0.5f}, {1.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f}, {1.0f, 0.0f, 1.0f}}
		};
		lveModel = std::make_unique<LveModel>(lveDevice, vertices);
	}

	void App::createPipeline() {
		auto pipelineConfig = LvePipeline::defaultPipelineConfigInfo(lveSwapChain->width(), lveSwapChain->height());
		pipelineConfig.renderPass = lveSwapChain->getRenderPass();
		pipelineConfig.layout = pipelineLayout;
		lvePipeline = std::make_unique<LvePipeline>(lveDevice, "./shader.vert.spv", "./shader.frag.spv", pipelineConfig);
	}

	void App::recreateSwapChain() {
		auto extent = lveWindow.getExtent();
		while (extent.width == 0 && extent.height == 0) {
			extent = lveWindow.getExtent();
			glfwWaitEvents();
		}
		vkDeviceWaitIdle(lveDevice.device());
		lveSwapChain = std::make_unique<MyEngineDevice>(lveDevice, extent);
		createPipeline();
	}

	void App::createPipelineLayout() {
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(lveDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create Pipeline!");
		}
	}

	void App::createcommadBuffers() {
		commandBuffer.resize(lveSwapChain->imageCount());

		VkCommandBufferAllocateInfo alloInfo{};
		alloInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		alloInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		alloInfo.commandPool = lveDevice.getCommandPool();
		alloInfo.commandBufferCount = static_cast<uint32_t>(commandBuffer.size());

		if (vkAllocateCommandBuffers(lveDevice.device(), &alloInfo, commandBuffer.data()) != VK_SUCCESS) {
			throw std::runtime_error("Failed to allocate command buffers!");
		}

	}

	void App::recordCommandBuffer(int imageIndex) {
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer[imageIndex], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("Failed to begin recording command buffer!");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = lveSwapChain->getRenderPass();
		renderPassInfo.framebuffer = lveSwapChain->getFrameBuffer(imageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = lveSwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValue{};
		clearValue[0].color = { 0.1f, 0.1f, 0.1f, 0.1f };
		clearValue[1].depthStencil = { 1.0f, 0 };         //check that later cuz something is wrong with number format
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValue.size());
		renderPassInfo.pClearValues = clearValue.data();

		vkCmdBeginRenderPass(commandBuffer[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		lvePipeline->bind(commandBuffer[imageIndex]);
		lveModel->bind(commandBuffer[imageIndex]);
		lveModel->draw(commandBuffer[imageIndex]);

		vkCmdDraw(commandBuffer[imageIndex], 3, 1, 0, 0);

		vkCmdEndRenderPass(commandBuffer[imageIndex]);
		if (vkEndCommandBuffer(commandBuffer[imageIndex]) != VK_SUCCESS) {
			throw std::runtime_error("Failed to record command buffer!");
		}
	}

	void App::drawFrame() {
		uint32_t imageIndex;
		auto result = lveSwapChain->acquireNextImage(&imageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapChain();
			return;
		}
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("Failed to acquire swap chain image!");
		}

		result = lveSwapChain->submitCommandBuffers(&commandBuffer[imageIndex], &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || lveWindow.windowResized()) {
			lveWindow.resetWindowResizedFlag();
			recreateSwapChain();
			return;
		}

		if (result != VK_SUCCESS) {
			throw std::runtime_error("Failed to present swap chain image!");
		}

	}
}