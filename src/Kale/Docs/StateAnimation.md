
@page stateAnimation Using the State Animation System

Kale has a Finite State Machine based animation system coded in. This basically means you can create your own enum containing a value for every "state" you want. A state is basically a representation of what you want your node to look like. Once you have provided Kale with your states and the values for every state, you will be able to animate between them easily. For example, look at the below @ref Kale::Transform "Transform" based state animation.

In the case of animating between states defined by transforms, you'll want to animate on a class which inherits from the @ref Kale::Transformable "Transformable" class. One example of this is a path node. A simple example has been provided below:

```cpp
// Somewhere in your header file:
// This is just an example enum, but the point is you should be representing each state of your node with an enum value
// for example, you could have a jump state, several run states, etc
enum class NodeState {
	First, Second
};

// All the following code should be placed in your onPresent scene event:

// Create a shared pointer to a pathnode which has no fill and a stroke. It also only has one bezier
Kale::Path path({Kale::CubicBezier{{0.0f, 0.0f}, {0.0f, 200.0f}, {500.0f, 200.0f}, {500.0f, 0.0f}}});
std::shared_ptr<Kale::PathNode> node = std::make_shared<Kale::PathNode>(path, false, Kale::PathNode::StrokeStyle::Both);

// Bring the node to the center to start
node->transform.translate(sceneBounds.center());

// Add the node to the scene
addNode(node);



// transformFSM is a std::optional which needs to be set to tell Kale you're using the state animation system.
node->transformFSM = std::make_optional<Kale::StateAnimatable<Kale::Transform>>();

// Then we add the data for two states, this simply uses two transforms one with a scale of 0 and the other with a scale of 1.
node->transformFSM->addStructure(NodeState::First, Kale::Transform({0.0f, 0.0f}, 0.0f, {0.0f, 0.0f}, Kale::AngleUnit::Radian));
node->transformFSM->addStructure(NodeState::Second, Kale::Transform({0.0f, 0.0f}, 0.0f, {1.0f, 1.0f}, Kale::AngleUnit::Radian));

// Now we can set the state to the second (scale of 1), this is done immediately
node->transformFSM->setState(NodeState::Second);

// And then animate infinitely in a loop between the first and second states with 4 seconds to each state
// What this achieves is an infinite animation of the object increasing and decreasing in scale
node->transformFSM->animateLoop<NodeState>({
	std::make_pair(NodeState::First, 4.0f),
	std::make_pair(NodeState::Second, 4.0f)
});

```

Transform is just one example of a structure capable of using state based animations. Other examples include paths themselves, and skeletons. You can also extend the system yourself with your own class.
