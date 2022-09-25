
@page bezier Cubic Beziers & Bezier Paths

Cubic beziers are curves which start from a starting point, are influenced by two control points, and end at a set point. The wikipedia [page](https://en.wikipedia.org/wiki/Bézier_curve) describes the math regarding them in further detail. The @ref Kale::CubicBezier "CubicBezier" struct in Kale has 4 @ref Kale::Vector2f "Vector2f" structs for each point in the bezier.

@ref Kale::Path "Paths" are made up of cubic beziers (they have a vector of @ref Kale::Path#beziers "beziers"). They can be either closed or open (to close it is to simply ensure the final bezier end point is the same as the first starting point). You can also get their bounding boxes for quick collision detection.
