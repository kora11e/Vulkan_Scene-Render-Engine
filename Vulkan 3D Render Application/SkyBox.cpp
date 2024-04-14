#include <ktx.h>
#include <glm/glm.hpp>

class SkyboxLoader {
public:
	bool displaySkybox = true;

	vks::Texture cubemap;

	struct Models {
		vkGLTF::Model skybox;

		std::vector<Model> objects;

		int32_t objectIndex = 0;
	} models;

	struct UBOVS {
		glm::mat4 projection;
		glm::mat4 modelView;
		glm::mat4 inverseModelView;
		float lodBias = 0.f;
	} uboVS;

	vks::Buffer uniformBuffer;

	struct {
		VkPipeline skybox{ VK_NULL_HANDLE };
		VkPipeline reflection{ VK_NULL_HANDLE };
	} pipelines;

	VkPipelineLayout pipelineLayout{ VK_NULL_HANDLE };
	VkDescriptorSet descriptorSet{ VK_NULL_HANDLE };
	VkDescriptorSetLayout descriptorSetLayout{ VK_NULL_HANDLE };

	std::vector<std::string> objectNames;


};