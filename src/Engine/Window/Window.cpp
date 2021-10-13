/*
   Copyright 2021 Rishi Challa

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

	   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "Window.hpp"
#include <Engine/Application/Application.hpp>
#include <vector>

using namespace Islands;

/**
 * Starts listening to events, override the functions provided in EventHandler to handle the events
 */
void Window::registerEvents(EventHandler* handler) {
	eventHandlers.push_back(handler);
}

/**
 * Stops listening to events
 */
void Window::removeEvents(EventHandler* handler) {
	eventHandlers.remove(handler);
}

/**
 * Creates the vulkan instance for this window
 */
void Window::createVulkanInstance() {
	vk::ApplicationInfo appInfo;
	appInfo.sType = vk::StructureType::eApplicationInfo;
	appInfo.pApplicationName = getTitle();
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_2;

	vk::InstanceCreateInfo createInfo;
	createInfo.sType = vk::StructureType::eInstanceCreateInfo;
	createInfo.pApplicationInfo = &appInfo;
	getCreateInfoExtensions(createInfo);
	createInfo.enabledLayerCount = 0;

	if (vk::createInstance(&createInfo, nullptr, &vulkanInstance) != vk::Result::eSuccess) {
		error("Unable to Init Vulkan");
		exit(0);
	}
}

void Window::cleanupVulkan() {
	vulkanInstance.destroy();
}

/**
 * Initializes vulkan for use with both windowing APIs
 */
void Window::initVulkan() {
	createVulkanInstance();
}