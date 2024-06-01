#pragma once

#include "lve_device.h"
#include "lve_Buffer.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <memory>
#include <vector>

namespace lve {
	class LveModel {
	public:
		struct Vertex {
			glm::vec2 position{};
			glm::vec3 color{};
			glm::vec3 normal{};
			glm::vec2 uv{};

			static std::vector<VkVertexInputBindingDescription> getBindingDescription();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescription();

			bool operator=(const Vertex& other) const {
				return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
			}
		};

		struct Builder {
			std::vector<Vertex> vertices{};
			std::vector<uint32_t> indices{};

			void loadModel(const std::string& filepath);
		};

		LveModel(MyEngineDevice& device, const std::vector<Vertex>& vertices);
		~LveModel();

		LveModel(const LveModel&) = delete;
		LveModel& operator=(const LveModel&) = delete;

		static std::unique_ptr<LveModel> createModelFromFile(MyEngineDevice& device, const std::string& filepath);

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);

	private:
		void createVertexBuffers(const std::vector<Vertex>& vertices);
		void createIndexBuffers(const std::vector<Vertex>& indices);

		MyEngineDevice& lveDevice;
		
		std::unique_ptr<LveBuffer> vertexBuffer;
		uint32_t vertexCount;

		bool hasIndexBuffer = false;

		std::unique_ptr<LveBuffer> indexBuffer;
		uint32_t indexCount;
	};
}