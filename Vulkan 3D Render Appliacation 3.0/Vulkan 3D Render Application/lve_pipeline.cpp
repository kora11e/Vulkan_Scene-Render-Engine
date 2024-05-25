#include "lve_pipeline.h"

#include <fstream>
#include <string>
#include <iostream>

namespace lve {

	LvePipeline::LvePipeline(MyEngineDevice& device, const std::string& vertFilepath, const std::string& fragpath, const PipelineConfigInfo& configInfo) : lveDevice{device} {
		createGraphicsPipeline(vertFilepath, fragpath, configInfo);
	}

	std::vector<char> LvePipeline::readFile(const std::string& filepath) {
		std::ifstream file{ filepath, std::ios::ate | std::ios::binary };

		if (!file.is_open()) {
			throw std::runtime_error("Failed to open file: " + filepath);
		}

		size_t fileSize = static_cast<size_t>(file.tellg());
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();
		return buffer;
	}

	void LvePipeline::createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragpath, const PipelineConfigInfo& configInfo) {
		auto vertCode = readFile(vertFilepath);
		auto fragCode = readFile(fragpath);

		std::cout << "Vertex Shader Code Size: " << vertCode.size() << '\n';
	}
}