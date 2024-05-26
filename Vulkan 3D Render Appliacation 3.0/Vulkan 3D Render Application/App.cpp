#include "App.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

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
		}
	}

	void App::createPipeline() {
		auto pipelineConfig = LvePipeline::defaultPipelineConfigInfo(lveSwapChain.width(), lveSwapChain.height());
		pipelineConfig.renderPass = lveSwapChain.getRenderPass();
		pipelineConfig.layout = pipelineLayout;
		lvePipeline = std::make_unique<LvePipeline>(lveDevice, "shader.vert.spv", "shader.frag.spv", pipelineConfig);
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

	}

	void App::drawFrame() {

	}
}