#pragma once

#include "lve_camera.h"
#include "lve_device.h"
#include "lve_frame_info.h"
#include "gameObject.h"
#include "lve_pipeline.h"

// std
#include <memory>
#include <vector>

namespace lve {
    class PointLightSystem {
    public:
        PointLightSystem(
            MyEngineDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~PointLightSystem();

        PointLightSystem(const PointLightSystem&) = delete;
        PointLightSystem& operator=(const PointLightSystem&) = delete;

        void update(FrameInfo& frameInfo, GlobalUbo& ubo);
        void render(FrameInfo& frameInfo);

    private:
        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createPipeline(VkRenderPass renderPass);

        MyEngineDevice& lveDevice;

        std::unique_ptr<LvePipeline> lvePipeline;
        VkPipelineLayout pipelineLayout;
    };
}  // namespace lve