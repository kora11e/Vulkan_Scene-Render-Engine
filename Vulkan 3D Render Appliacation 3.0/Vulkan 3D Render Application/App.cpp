#include "App.h"

#include "lve_renderer.h"
#include "Simple_render_system.h"

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

	App::App() { loadgameObjects(); }

	App::~App() {}

	void App::run() {
		RenderSystem renderSystem{ lveDevice, lveRenderer.getSwapChainRenderPass() };

		while (!lveWindow.shouldClose()) {
		
			glfwPollEvents();

			if (auto commandBuffer = lveRenderer.beginFrame()) {
				lveRenderer.beginSwapChainRenderPass(commandBuffer);
				renderSystem.renderGameObjects(commandBuffer, gameObjects);
				lveRenderer.endSwapChainRenderPass(commandBuffer);
				lveRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(lveDevice.device());
	}

	void App::loadgameObjects() {
		std::vector<LveModel::Vertex> vertices{
			{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, 0.5f}, {1.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f}, {1.0f, 0.0f, 1.0f}}
		};
		auto lveModel = std::make_shared<LveModel>(lveDevice, vertices);

		auto triangle = LveGameObject::createGameObject();
		triangle.model = lveModel;
		triangle.color = {.1f, .8f, .1f};
		triangle.transform2d.translation.x = .2f;
		triangle.transform2d.scale = {2.f, .5f};
		triangle.transform2d.rotation = .25f * glm::two_pi<float>();

		gameObjects.push_back(std::move(triangle));
	}
}