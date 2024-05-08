#pragma once

#ifndef _TRANSFORMATIONS_
#define _TRANSFORMATIONS_
#define GLFW_INCLUDE_VULKAN

#include "GLFW/glfw3.h"
#include <glm/glm.hpp>

class Transformations {
public:
	Transformations();
	Transformations(const Transformations&);
	~Transformations();

	void GetProjectionMatrix(glm::mat4x4&);
	void GetWorldMatrix(glm::mat4x4&);
	void GetOrthoMatrix(glm::mat4x4&);

	void ResetViewport();

	void TurnZBufferOn();
	void TurnZBufferOff();
private:

};

#endif