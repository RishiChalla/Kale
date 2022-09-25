
@page coordinateSystem The Kale Coordinate System

The Kale coordinate system was designed to be as simplistic as possible. Since Kale is a vector based game engine, all rendering is resolution independent. But for all designing to be resolution independent, a single resolution must be targetted, hence the choice of 1080p. The coordinate system used makes it so all rendering is always done with a size of 1920 x 1080 regardless of actual resolution. This means centering a node at (960, 540) will keep the node centered regardless of aspect ratio. X coordinates increase from left to right, and y coordinates increase from bottom to top.

> **Note** - Due to the scaling of aspect ratios, if the actual resolution aspect ratio is wider than 16:9, then nodes which are offscreen at 16:9 1080p will be rendered. Additionally, if the actual aspect ratio is more narrow than 16:9, then nodes which are typically rendered at 16:9 1080p will not be rendered. We recommend accounting for devices ranging from 20:9 to 4:3. This will account for all modern smartphones with tall screens, to old CRT monitors.

A bounding box of the screen coordinates is available via @ref Kale::Scene#getSceneBounds "mainApp->getPresentedScene()->getSceneBounds()". The pure width and height of the output at 1080p is also available via @ref Kale::Scene#getViewport "mainApp->getPresentedScene()->getViewport()" (The height of the viewport will always be 1080p, the width will change depending on the aspect ratio).

@TODO - Will add mechanisms to set a maximum & minimum ratio to avoid unwanted rendering
