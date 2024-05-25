#pragma once

#include "lve_window.h"
#include <vector>
#include <string>

namespace lve{
	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilites;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	struct QueueFamilyIndices {
		uint32_t graphicsFamily;
		uint32_t presentFamily;
		bool graphicsFamilyHasValue = false;
		bool presentFamilyHasValue = false;
		bool isComplete() { return graphicsFamilyHasValue && presentFamilyHasValue;  };
	};

	class MyEngineDevice {
	public:
		#ifndef NDEBUG
		const bool enableValidationLayers = false;
		#else
	};
}