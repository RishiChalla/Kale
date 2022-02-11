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

#include <Kale/Core/Logger/Logger.hpp>

#include <stdexcept>

#include <gpu/GrBackendSurface.h>
#include <gpu/gl/GrGLInterface.h>
#include <core/SkCanvas.h>
#include <core/SkColorSpace.h>
#include <core/SkData.h>
#include <core/SkImage.h>
#include <core/SkStream.h>

using namespace Kale;

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
 * Recreates the skia surface (should be called on window resize)
 */
void Window::recreateSkiaSurface() {
	Vector2i size(getSize().cast<int>());

	auto interface = GrGLMakeNativeInterface();
	skiaContext = GrDirectContext::MakeGL(interface).release();

	if (skiaContext == nullptr) {
		console.error("Unable to create Skia Context");
		exit(0);
	}

	GrGLFramebufferInfo framebufferInfo;
	framebufferInfo.fFBOID = 0;
	framebufferInfo.fFormat = GL_RGBA8;

	SkColorType colorType = kRGBA_8888_SkColorType;
	GrBackendRenderTarget backendRenderTarget(size.x, size.y, 0, 0, framebufferInfo);

	skiaSurface = SkSurface::MakeFromBackendRenderTarget(skiaContext, backendRenderTarget, GrSurfaceOrigin::kBottomLeft_GrSurfaceOrigin,
		colorType, nullptr, nullptr).release();
	
	if (skiaSurface == nullptr) {
		console.error("Unable to create Skia Surface");
		exit(0);
	}
}

/**
 * Gets the canvas to draw to
 * @throws If the canvas does not currently exist
 * @returns The canvas
 */
SkCanvas& Window::getCanvas() {
	if (skiaSurface == nullptr) throw std::runtime_error("Skia Surface Does not Exist (Canvas Retrieval Attempt)");
	return *skiaSurface->getCanvas();
}
