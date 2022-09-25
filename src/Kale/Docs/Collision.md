
@page collision Using the Math Basic Collision Detection Modules

The Math module of Kale has a few different geometrical objects capable of collision detection:

 - @ref Kale::Circle "Circle"
 - @ref Kale::Line "Line"
 - @ref Kale::Ray "Ray"
 - @ref Kale::Rect "Rect"
 - @ref Kale::RotatedRect "RotatedRect"

All of these inherit from the @ref Kale::Geometry "Geometry" class. All but rays can get bounding boxes (rays are infinite), and all have collision detection methods coded for testing with eachother. Where possible, it is recommended to use bounding boxes for collision detection prior to using any other methods as they are the cheapest in terms of performance.
