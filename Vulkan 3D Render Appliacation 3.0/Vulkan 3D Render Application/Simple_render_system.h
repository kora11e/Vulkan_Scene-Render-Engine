#pragma once

#include "lve_device.h"
#include "gameObject.h"
#include "lve_pipeline.h"

#include <memory>
#include <vector>

namespace lve {
	class RenderSystem {
	public:
		RenderSystem(MyEngineDevice& device, VkRenderPass renderPass);
		~RenderSystem();

		RenderSystem(const RenderSystem&) = delete;
		RenderSystem& operator=(const RenderSystem&) = delete;

		void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<LveGameObject>& gameObjects);

	private:
		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);

		MyEngineDevice& lveDevice;

		std::unique_ptr<LvePipeline> lvePipeline;
		VkPipelineLayout pipelineLayout;
	};
}