#pragma once

#include <memory>
#include "lve_model.h"
#include <unordered_map> 
#include "glm/gtc/matrix_transform.hpp"

namespace lve {

	struct TransformComponent {
		glm::vec3 translation{};
		glm::vec3 scale{1.f, 1.f, 1.f};
		glm::vec3 rotation{};

		glm::mat4 mat4();
		glm::mat3 normalMatrix();

		glm::mat2 scaleMat2() { //2d rotation - remove or whatever
			
			glm::mat2 scaleMat{ {scale.x, .0f}, {.0f, scale.y}};
			return glm::mat2{ 1.f }; 
		}
	};

	//inefficient for bigger projects
	class LveGameObject {
	public:
		using id_t = unsigned int;
		using Map = std::unordered_map<id_t, LveGameObject>;
	
		static LveGameObject createGameObject() {
			static id_t currentId = 0;
			return LveGameObject{currentId};
		}
		LveGameObject(const LveGameObject&) = delete;
		LveGameObject& operator=(const LveGameObject&) = delete;
		LveGameObject(LveGameObject&&) = default;
		LveGameObject& operator=(LveGameObject&&) = default;

		id_t getId() { return id; };

		std::shared_ptr<LveModel> model{};
		glm::vec3 color{};
		TransformComponent transform{};

	private:
		LveGameObject(id_t objId) : id{ objId } {}
			id_t id;
	};
}