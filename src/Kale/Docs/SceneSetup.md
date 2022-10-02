
@page sceneSetup Creating & Setting up a Scene

@section scenesetup_s1 Creating a Scene

To create a scene you need to inherit from the @ref Kale::Scene "Scene" class. A basic implementation consists of the following:

```cpp
// TestScene.hpp

#pragma once

#include <Kale/Core/Core.hpp>
#include <Kale/Engine/Engine.hpp>

namespace KaleTesting {

	class TestScene : public Kale::Scene {
	private:

	protected:
		void onPresent() override;

	public:

	};

}

```

```cpp
// TestScene.cpp

#include "TestScene.hpp"

using namespace KaleTesting;

void TestScene::onPresent() {
	Scene::onPresent();
	klPrint("Hello World!");
}
```

@section scenesetup_s2 Scene Events

The @ref Kale::Scene "Scene" class inherits from the @ref Kale::EventHandler "EventHandler" class and listens to events when presented. You can override any EventHandler events directly in your scene class.

> **NOTE** - If you override the @ref Kale::EventHandler#onWindowResize "EventHandler" event, make sure you call the @ref Kale::Scene#onWindowResize "Scene" super method!

In addition to the events inherited from EventHandler, the Scene class also has all the following inheritable events:

- @ref Kale::Scene#onPresent "void onPresent();" - Called when the scene is presented, guaranteed to be called from the main thread
- @ref Kale::Scene#onSceneChange "void onSceneChange();" - Called prior to the scene being changed, guaranteed to be called from the main thread
- @ref Kale::Scene#onUpdate "void onUpdate(size_t threadNum, float deltaTime);" - Called on every frame update, called on every thread
- @ref Kale::Scene#onPreUpdate "void onPreUpdate(size_t threadNum, float deltaTime);" - Called on every frame prior to the update, called on every thread

If you need to run some code every frame which isn't applicable to a node directly, you need to add an if statement to check for a specific thread to avoid any update code from being called on multiple threads at the same time. You also need to ensure that your threadNum is in the range of the target hardware supported number of threads if you hardcode this.

@section scenesetup_s3 Node System & Thread Scheduling

There are different types of nodes, each detailed more in the @ref tutorials "Tutorials & Guides" page. All nodes inherit from the @ref Kale::Node "Node" class. To add a node, you need to pass a shared pointer of the node to @ref Kale::Scene#addNode "addNode". The method is templated and will work with any type inherited from the node class.

Kale uses a thread pool for updates. The number of threads in the pool is equivelant to `std::thread::hardware_concurrency()` (basically the number of threads supported on the device). Since there is a variable amount of threads, there is a node scheduling algorithm used to determine which thread a node is updated and pre updated on. Generally speaking, the timeframe for every render frame looks like the below:

```
All update threads Pre Update Nodes
** Wait for all threads to finish **
All update threads Update Nodes
** Wait for all threads to finish **
Nodes which need to be added/removed are updated.
Main thread renders, all update threads wait for rendering to finish
Main thread waits for next frame's update threads.
```

Each node has an @ref Kale::Node#updateTime "updateTime" and @ref Kale::Node#preUpdateTime "preUpdateTime" used for assigning nodes to threads. This means that a node can be pre updated in a different thread than it is updated in. Nodes have the following events:

- @ref Kale::Node#begin "void begin(const Scene& scene)" - Called when the node is added to the scene, gauranteed to be called from the main thread
- @ref Kale::Node#end "void end(const Scene& scene)" - Called when the node is removed from the scene, gauranteed to be called from the main thread
- @ref Kale::Node#render "void render(const Camera& camera, float deltaTime) const" - Called when the node must be rendered to the display, guaranteed to be called from the main thread
- @ref Kale::Node#preUpdate "void preUpdate(size_t threadNum, const Scene& scene, float deltaTime)" - Called prior to updates. An excellent place to update properties of the node without reading from any other nodes
- @ref Kale::Node#update "void update(size_t threadNum, const Scene& scene, float deltaTime)" - Called after pre-updates. An excellent place to update the node if reading of other nodes is required.

There are times when you may need to run some code on the main thread after updating but before rendering. Kale has a task system which you can use for this. Simply pass a function to @ref Kale::Application#runTaskOnMainThread "mainApp->runTaskOnMainThread(fn);". Be careful when doing this however, if your function is a lambda which captures by reference or your function references any local variables, you'll want to ensure the variables you're referencing will be alive when the function is called.
