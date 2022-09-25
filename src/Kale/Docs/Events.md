
@page events Using the Events System & Responding to User Input

To use the events system, you simply need to inherit from the @ref Kale::EventHandler "EventHandler" class. From there, you can override any method you wish to receive input from. To actually begin receiving updates, you need to register your events handler to the window class using @ref Kale::Window::registerEvents "mainApp->getWindow().registerEvents();". Each event is described below:

> **Note** - @ref Kale::Window::registerEvents "registerEvents()" takes a raw pointer. If the class pointed to is destructed prior to being removed from events, it will cause a crash.
<br>

> **Note** - All event handler callbacks are guaranteed to be called from the main thread. This is done every frame prior to any updates/pre updates.

 - @ref Kale::EventHandler::onWindowResize "void onWindowResize(Vector2ui oldSize, Vector2ui newSize);" - Called when the window is resized, includes both the new and old window sizes in terms of pixels. You should avoid using this since all rendering is done relative to @ref coordinateSystem "1080p".
 - @ref Kale::EventHandler::onWindowLostFocus "void onWindowLostFocus();" - Called right before the window is about to lose focus. Depending on OS, updates/rendering will stop.
 - @ref Kale::EventHandler::onWindowGainedFocus "void onWindowGainedFocus();" - Called right before the window is about to regain focus. Depending on OS, updates/rendering will begin again.
 - @ref Kale::EventHandler::onControllerConnect "void onControllerConnect(unsigned int controller);" - Called when a controller connects, also gives the id of the controller
 - @ref Kale::EventHandler::onControllerDisconnect "void onControllerDisconnect(unsigned int controller);" - Called when a controller disconnects
 - @ref Kale::EventHandler::onControllerButtonPress "void onControllerButtonPress(unsigned int controller, ControllerButton button);" - Called when a button is pressed on a controller.
 - @ref Kale::EventHandler::onControllerButtonRelease "void onControllerButtonRelease(unsigned int controller, ControllerButton button);" - Called when a button is released on a controller
 - @ref Kale::EventHandler::onControllerHandle "void onControllerHandle(unsigned int controller, ControllerAxis handle, float position);" - Called when a controller handle is moved
 - @ref Kale::EventHandler::onControllerJoystick "void onControllerJoystick(unsigned int controller, ControllerAxis joystick, Vector2f position);" - Called when a controller joystick is moved
 - @ref Kale::EventHandler::onKeyPress "void onKeyPress(Key key);" - Called when the user presses a key
 - @ref Kale::EventHandler::onKeyRelease "void onKeyRelease(Key key);" - Called when the user releases a key
 - @ref Kale::EventHandler::onMouseMove "void onMouseMove(Vector2f pos);" - Called when the mouse moves with focus on the window (this can be outside of the window depending on OS)
 - @ref Kale::EventHandler::onMouseScroll "void onMouseScroll(float scroll);" - Called when the mosue is scrolled with focus on the window
 - @ref Kale::EventHandler::onLeftClick "void onLeftClick();" - Called when the user left clicks on the window
 - @ref Kale::EventHandler::onMiddleClick "void onMiddleClick();" - Called when the user middle clicks on the window (try to avoid using this without an alternative/rebinds for trackpad users)
 - @ref Kale::EventHandler::onRightClick "void onRightClick();" - Called when the user right clicks on the window
 - @ref Kale::EventHandler::onLeftClickRelease "void onLeftClickRelease();" - Called when the user releases their left click
 - @ref Kale::EventHandler::onMiddleClickRelease "void onMiddleClickRelease();" - Called when the user releases their middle click
 - @ref Kale::EventHandler::onRightClickRelease "void onRightClickRelease();" - Called when the user releases their right click
 - @ref Kale::EventHandler::onTouchBegin "void onTouchBegin(unsigned int touch, Vector2f pos);" - Called when touches begin (this doesn't necessarily apply only to mobile devices, also applies to window laptops with touch screens)
 - @ref Kale::EventHandler::onTouchMove "void onTouchMove(unsigned int touch, Vector2f pos);" - Called when touches change position
 - @ref Kale::EventHandler::onTouchEnd "void onTouchEnd(unsigned int touch);" - Called when a touch is released

> **Note** - Depending on your compile settings, these events are directly passed from GLFW or SDL2. GLFW is known to be more lightweight, but lacks some features such as controller databasing. This means that controller binds to enum values may be incorrect when using a GLFW build.
