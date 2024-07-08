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
    class RenderSystem {
    public:
        RenderSystem(
            MyEngineDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~RenderSystem();

        RenderSystem(const RenderSystem&) = delete;
        RenderSystem& operator=(const RenderSystem&) = delete;

        void renderGameObjects(FrameInfo& frameInfo);

    private:
        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createPipeline(VkRenderPass renderPass);

        MyEngineDevice& lveDevice;

        std::unique_ptr<LvePipeline> lvePipeline;
        VkPipelineLayout pipelineLayout;
    };
}  // namespace lve