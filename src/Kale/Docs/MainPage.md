
@mainpage Kale Documentation

@section mainpage_s1 What is Kale?
Kale is a 2D Game Engine made with Vulkan with specialized "blobby" graphics. Kale supports Mac OSX, Windows, Linux,
~~IOS~~, ~~Android~~, and the ~~Nintendo Switch~~ (Striked platforms currently do not have support but are planned in
the near future). This guide will teach you how to get back on your feet and use Kale for development if you ever forget.

TODO - Finish this section after the editor is created

@section mainpage_s2 Getting Started - Code/CMake
Create a new directory for your project and create a new file called `CMakeLists.txt` and a new directory for your source code.
Go inside your new directory and clone the github repo - 
```shell
cd YourProjectDirectory
git clone https://github.com/RishiChalla/Kale --recursive
```

Then in your parent directory paste the following in your `CMakeLists.txt` and change the project name for your project.
```cmake
cmake_minimum_required(VERSION 3.0.0)
project(KaleTesting VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS src/*.cpp src/*.hpp)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS src/*.cpp src/*.hpp)
add_executable(KaleTesting ${sources})
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${sources})

target_include_directories(KaleTesting PUBLIC src/)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# Kale
add_subdirectory(Kale)
target_link_libraries(KaleTesting Kale)

# Set the assets folder
set(assetsOutputFolder $<TARGET_FILE_DIR:${PROJECT_NAME}>/.KaleTesting/assets)

# Copy all resources (assets, shaders, etc)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/assets ${assetsOutputFolder})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					   ${CMAKE_SOURCE_DIR}/Kale/assets ${assetsOutputFolder})
```

Add an src folder and create two files: `App.hpp` and `App.cpp` within it as a simple test. Copy the code below into each file to ensure
Kale is properly linked and working. Be sure to also create an assets folder (this can be empty for now) in your parent directory.

> App.hpp
```cpp
#pragma once

#include <Kale/Core/Core.hpp>

namespace KaleTesting {
	
	// Represents the main application class
	class App : public Kale::Application {

	protected:
		
		// Called when the application begins
		void onBegin() override;
	
	public:

		// Creates a new app instance
		App();
	};
}
```

> App.cpp

```cpp
#include "App.hpp"

using namespace KaleTesting;

// Heap allocates the application
Kale::Application* createApplication() {
	return new App();
}

// Creates a new app instance
App::App() : Kale::Application("KaleTesting") {
	// Empty constructor - nothing to do here.
}

// Called when the application begins
void App::onBegin() {
	klPrint("Hello world!");
}
```

@section mainpage_s3 Next Steps
Have a look at the pages and guides for learning how to use specific parts of the kale engine. Specifically see the tutorials and guides listed @ref tutorials "here". You may also want to look specifically at @ref sceneSetup "setting up a scene" as a window may become unresponsive without a presented scene.
