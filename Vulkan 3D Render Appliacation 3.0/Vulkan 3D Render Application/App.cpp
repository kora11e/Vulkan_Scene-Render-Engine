#include "App.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <array>

namespace lve {

	struct PushDataConstant {
		glm::vec2 offset;
		alignas(16) glm::vec3 color;
	};

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
		PipelineConfigInfo pipelineConfig{};
		LvePipeline::defaultPipelineConfigInfo(pipelineConfig);
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

		if (lveSwapChain == nullptr) {
			lveSwapChain = std::make_unique<MyEngineDevice>(lveDevice, extent);
		}
		else {
			lveSwapChain = std::make_unique<MyEngineDevice>(lveDevice, extent, std::move(lveSwapChain));
			if (lveSwapChain->imageCount() != commandBuffer.size()) {
				freeCommandBuffers();
			}
		}
		createPipeline();
	}

	void App::createPipelineLayout() {

		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(PushDataConstant);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

		if (vkCreatePipelineLayout(lveDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create Pipeline!");
		}
	}

	void App::freeCommandBuffers() {
		vkFreeCommandBuffers(lveDevice.device(), lveDevice.getCommandPool(), static_cast<float>(commandBuffer.size()), commandBuffer.data());
		commandBuffer.clear();
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
		static int frame = 0;
		frame = (frame + 1) & 1000;

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

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<uint32_t>(lveSwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<uint32_t>(lveSwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, lveSwapChain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffer[imageIndex], 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer[imageIndex], 0, 1, &scissor);

		lvePipeline->bind(commandBuffer[imageIndex]);
		lveModel->bind(commandBuffer[imageIndex]);

		for (int i = 0; i < 4; i++) {
			PushDataConstant push{};
			push.offset = { -0.5f + frame * 0.002f, -0.4f +i* 0.25f };
			push.color = { 0.0f, 0.0f, 0.2f + 0.2f * i };

			vkCmdPushConstants(commandBuffer[imageIndex], pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushDataConstant), &push);
			lveModel->draw(commandBuffer[imageIndex]);
		}

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