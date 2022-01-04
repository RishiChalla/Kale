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

#pragma once

#include <map>

#include <glad/glad.h>

namespace Kale::OpenGL {

	/**
	 * Converts a OpenGL based enum to the GLEnum/unsigned int value
	 * @param value The value of the enum type to convert
	 */
	template <typename T> GLenum getEnumValue(T value) {
		return static_cast<GLenum>(value);
	}

#ifdef KALE_DEBUG

	inline const std::map<GLenum, const char*> enumStringValueMap = {
		{GL_DEBUG_SOURCE_API, "OpenGL API"},
		{GL_DEBUG_SOURCE_APPLICATION, "Application"},
		{GL_DEBUG_SOURCE_OTHER, "Other"},
		{GL_DEBUG_SOURCE_SHADER_COMPILER, "Shader Compiler"},
		{GL_DEBUG_SOURCE_THIRD_PARTY, "Third Party"},
		{GL_DEBUG_SOURCE_WINDOW_SYSTEM, "Window System"},

		{GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "Deprecated Behavior"},
		{GL_DEBUG_TYPE_ERROR, "Error"},
		{GL_DEBUG_TYPE_MARKER, "Marker"},
		{GL_DEBUG_TYPE_OTHER, "Other"},
		{GL_DEBUG_TYPE_PERFORMANCE, "Performance"},
		{GL_DEBUG_TYPE_POP_GROUP, "Pop Group"},
		{GL_DEBUG_TYPE_PORTABILITY, "Portability"},
		{GL_DEBUG_TYPE_PUSH_GROUP, "Push Group"},
		{GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, "Undefined Behavior"},

		{GL_DEBUG_SEVERITY_HIGH, "High"},
		{GL_DEBUG_SEVERITY_LOW, "Low"},
		{GL_DEBUG_SEVERITY_MEDIUM, "Medium"},
		{GL_DEBUG_SEVERITY_NOTIFICATION, "Notification"}
	};

#endif

}
