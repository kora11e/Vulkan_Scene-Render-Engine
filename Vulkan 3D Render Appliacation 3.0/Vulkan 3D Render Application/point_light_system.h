#pragma once

#include "lve_device.h"
#include "gameObject.h"
#include "lve_pipeline.h"
#include "lve_camera.h"
#include "lve_frame_info.h"

#include <memory>
#include <vector>

namespace lve {
	class PointLightSystem {
	public:
		PointLightSystem(MyEngineDevice& device, VkRenderPass renderPass);
		~PointLightSystem();

		PointLightSystem(const PointLightSystem&) = delete;
		PointLightSystem& operator=(const PointLightSystem&) = delete;

		void renderGameObjects(FrameInfo& frameInfo, std::vector<LveGameObject>& gameObjects);

	private:
		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);

		MyEngineDevice& lveDevice;

		std::unique_ptr<LvePipeline> lvePipeline;
		VkPipelineLayout pipelineLayout;
	};
}