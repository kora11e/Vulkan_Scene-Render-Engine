#include "App.h"

#include "lve_window.h"
#include "lve_renderer.h"
#include "Simple_render_system.h"
#include "lve_camera.h"
#include "lve_Buffer.h"
#include "lve_Descriptors.h"
#include "point_light_system.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <array>
#include <cassert>
#include <chrono>
#include <numeric>
#include <memory>

namespace lve {

    struct GlobalUbo {
        glm::mat4 projection{1.f};
        glm::mat4 view{ 1.f };
        glm::vec4 ambientLightColor{1.f, 1.f, 1.f, .02f};
        glm::vec3 lightPosition{-1.f};
        alignas(16) glm::vec4 lightColor{1.f};
    };

	App::App() { 
        auto globalPool = LveDescriptorPool::Builder(lveDevice) // typ siê zepsu³ czy coœ idk
            .setMaxSets(MyEngineSwapChain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, MyEngineSwapChain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, MyEngineSwapChain::MAX_FRAMES_IN_FLIGHT)
            .build();
        loadgameObjects();    
    }

	App::~App() {}

	void App::run() {
    
        std::vector<std::unique_ptr<LveBuffer>> uboBuffers(MyEngineSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < uboBuffers.size(); i++) {
            uboBuffers[i] = std::make_unique<LveBuffer>(
                    lveDevice,
                    sizeof(GlobalUbo),
                    MyEngineSwapChain::MAX_FRAMES_IN_FLIGHT,
                    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                );
            uboBuffers[i]->map();
        }

        auto globalSetLayout = LveDescriptorSetLayout::Builder(lveDevice)
            .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
            .build();

        std::vector<VkDescriptorSet> globalDescriptorSets(MyEngineSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < globalDescriptorSets.size(), i++) {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            LveDescriptorWriter(*globalSetLayout, *globalPool)
                .writeBuffer(0, &bufferInfo)
                .build(globalDescriptorSets[i]);
        }

        LveBuffer globalUboBuffer{
            lveDevice,
            sizeof(GlobalUbo),
            MyEngineSwapChain::MAX_FRAMES_IN_FLIGHT,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
            lveDevice.properties.limits.minUniformBufferOffsetAlignment,
        };
        globalUboBuffer.map();


		RenderSystem renderSystem{ lveDevice, lveRenderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout() };

        PointLightSystem pointLightSystem{ lveDevice, lveRenderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout() };


        LveCamera camera{};
        //camera.setViewDirection(glm::vec3(0.f), glm::vec3(.5f, 0.f, 1.f));
        camera.setViewTarget(glm::vec3(-1.f, -2.f, 2.f), glm::vec3(0.f, 0.f, 2.5f));

        auto viewerObject = LveGameObject::createGameObject();

        auto currentTime = std::chrono::high_resolution_clock::now();

		while (!lveWindow.shouldClose()) {

            float aspect = lveRenderer.getAspectRatio();
			glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            frameTime = glm::min(frameTime, MAX_FRAME_TIME);

            camera.moveInPlaneXZ(lveWindow.getGLFWwindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            //camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);


			if (auto commandBuffer = lveRenderer.beginFrame()) {

                int frameIndex = lveRenderer.getFrameIndex();

                FrameInfo frameInfo{
                    frameIndex,
                    frameTime,
                    commandBuffer,
                    camera,
                    globalDescriptorSets[frameIndex],
                    gameObjecs
                };

                GlobalUbo ubo{};
                ubo.projection = camera.getProjection();
                ubo.view = camera.getView();
                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush();

				lveRenderer.beginSwapChainRenderPass(commandBuffer);
				renderSystem.renderGameObjects(frameInfo);
                pointLightSystem.render(frameInfo);
				lveRenderer.endSwapChainRenderPass(commandBuffer);
				lveRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(lveDevice.device());
	}

	void App::loadgameObjects() {
        std::shared_ptr<LveModel> lveModel = LveModel::createModelFromFile(lveDevice, "./smooth_vase.obj");

        auto smoothVase = LveGameObject::createGameObject();
        smoothVase.model = lveModel;
        smoothVase.transform.translation = {0.f, 0.f, .5f};
        smoothVase.transform.scale = {.5f, .5f, .5f};
        gameObjecs.emplace(smoothVase.getId(),std::move(smoothVase));

        lveModel = LveModel::createModelFromFile(lveDevice, "./Quad.obj");

        auto Quad = LveGameObject::createGameObject();
        Quad.model = lveModel;
        Quad.transform.translation = { 0.f, 0.f, 1.5f };
        Quad.transform.scale = { .5f, .5f, .5f };
        gameObjecs.emplace(Quad.getId(), std::move(Quad));

        lveModel = LveModel::createModelFromFile(lveDevice, "./przybornik.obj");

        auto przybornik = LveGameObject::createGameObject();
        przybornik.model = lveModel;
        przybornik.transform.translation = { 0.f, 0.f, -1.5f };
        przybornik.transform.scale = { .5f, .5f, .5f };
        gameObjecs.emplace(przybornik.getId(), std::move(przybornik));
    }
}