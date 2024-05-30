#pragma once

#include "lve_window.h"
#include "lve_pipeline.h"
#include "lve_device.h"
#include "gameObject.h"
#include "Swapchain.h"
#include "lve_renderer.h"

#include <memory>
#include <vector>

namespace lve {
	class App {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		App();
		~App();

		App(const LveWindow&) = delete;
		App& operator=(const LveWindow&) = delete;

		void run();
	private:
		void loadgameObjects();

		LveWindow lveWindow{ WIDTH, HEIGHT, "Vulkan Renderer!"};
		MyEngineDevice lveDevice{lveWindow};
		LveRenderer lveRenderer{lveWindow, lveDevice};

		std::vector<LveGameObject> gameObjects;
	};

}